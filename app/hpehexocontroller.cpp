#include "hpehexocontroller.h"

#include <iostream>
#include <cstdarg>
#include <csignal>

#include "Dialogs/hpeprocessingdialog.h"
#include "Terminal/qterminalprocess.h"

HPEHexoController::HPEHexoController(QTerminalProcess *process,
                                     const QDir &targetDir,
                                     QObject *parent)
    : QObject{parent}, m_workingDir(targetDir)
{
    if(process && process->getExecutablePaths().contains("npx"))
        NPX_PATH = QFileInfo(process->getExecutablePaths().value("npx")).absolutePath();

    m_commmadProcess = new QProcess();
    m_commmadProcess->setWorkingDirectory(m_workingDir.absolutePath());
    m_commmadProcess->setProgram(NPX_PROGRAM);

    m_serverProcess = new QProcess();
    m_serverProcess->setWorkingDirectory(m_workingDir.absolutePath());
    m_serverProcess->setProgram(NPX_PROGRAM);

    m_processingDialog = new HPEProcessingDialog("");

    //try to find npx
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //set this application's env so that it can search apps in env
    QStringList newPaths = env.value("PATH").split(QDir::listSeparator());
    newPaths.append(NPX_PATH);
    newPaths.removeDuplicates();
    QString newPathString = newPaths.join(QDir::listSeparator());
    qputenv("PATH", qPrintable(newPathString));
    env.insert("PATH", newPathString);
    m_commmadProcess->setProcessEnvironment(env);
    m_serverProcess->setProcessEnvironment(env);

    bindingProcessingDialogEvents();
    bindingServerEvents();

    //checkHexoInstallation();
}

HPEHexoController::~HPEHexoController()
{
    stopServer();
    m_serverProcess->terminate();
    m_serverProcess->kill();
    m_serverProcess->deleteLater();
    m_commmadProcess->terminate();
    m_commmadProcess->kill();
    m_commmadProcess->deleteLater();
}

void HPEHexoController::setDir(const QDir &dir)
{
    if(!dir.exists())
        return;

    m_workingDir = dir;
    m_commmadProcess->setWorkingDirectory(m_workingDir.absolutePath());
    m_serverProcess->setWorkingDirectory(m_workingDir.absolutePath());
    checkHexoInstallation();
}

bool HPEHexoController::createPost(const QString &title, const QString &layout)
{
    m_processingDialog->setPrompt(tr("Creating post %1").arg(title));
    if(!isCommandProcessAvailable())
        return false;

    m_commmadProcess->setArguments(HEXO_ARGUMENTS.value(HPEHexoController::CREATE) << layout << title);

    disconnect(m_commmadProcess, nullptr, this, nullptr);  //remove previous listeners
    connect(m_commmadProcess, &QProcess::finished, this, [this] {
        QString res(m_commmadProcess->readAllStandardOutput());
        QString error(m_commmadProcess->readAllStandardError());

        //ADD SETTING: strict error processing
        if(!error.isEmpty() && (error.contains("FATAL", Qt::CaseInsensitive) ||
                                error.contains("ERROR", Qt::CaseInsensitive)))
            emit commandProcessError(error);
        else if(res.contains("FATAL", Qt::CaseInsensitive) ||
                res.contains("ERROR", Qt::CaseInsensitive))
            emit commandProcessError(res);
        else
            emit processFinished(res, HPEHexoController::CREATE);
    });

    m_commmadProcess->start(QProcess::ReadOnly);
    emit commandProcessStart();
    return true;
}

bool HPEHexoController::generate()
{
    m_processingDialog->setPrompt(tr("Generating"));
    return processCommand(GENERATE);
}

bool HPEHexoController::clean()
{
    m_processingDialog->setPrompt(tr("Cleaning"));
    return processCommand(CLEAN);
}

bool HPEHexoController::launchServer()
{
    if(!m_hexoInstalled)
        return false;

    if(m_serverProcess->state() == QProcess::Running)
        return true;

    m_serverProcess->setArguments(HEXO_ARGUMENTS.value(SERVER));
    m_serverProcess->start(QProcess::ReadOnly);
    return true;
}

void HPEHexoController::stopServer()
{
    if(m_serverProcess->state() == QProcess::Running)
        kill(m_serverProcess->processId(), SIGINT);   //quit server
}

bool HPEHexoController::deploy()
{
    m_processingDialog->setPrompt(tr("Deploying"));
    return processCommand(DEPLOY);
}

void HPEHexoController::bindingProcessingDialogEvents()
{
    connect(this, &HPEHexoController::commandProcessStart, m_processingDialog, &HPEProcessingDialog::exec);
    connect(this, &HPEHexoController::hexoEnvironmentChecked, m_processingDialog, &HPEProcessingDialog::accept);
    connect(this, &HPEHexoController::processFinished, m_processingDialog, &HPEProcessingDialog::accept);
    connect(this, &HPEHexoController::unknownError, m_processingDialog, &HPEProcessingDialog::showError);
    connect(this, &HPEHexoController::npxNotFound,  m_processingDialog, &HPEProcessingDialog::showError);
    connect(this, &HPEHexoController::hexoNotFound,  m_processingDialog, &HPEProcessingDialog::showError);
    connect(this, &HPEHexoController::commandProcessError, m_processingDialog, &HPEProcessingDialog::showError);
    //connect(this, &HPEHexoController::commandProcessError, this, &HPEHexoController::stopServer);
}

void HPEHexoController::bindingServerEvents()
{
    connect(m_serverProcess, &QProcess::readyReadStandardOutput, this, [this] {
        QString res(m_serverProcess->readAll());
        if(res.contains("FATAL", Qt::CaseInsensitive) ||
            res.contains("ERROR", Qt::CaseInsensitive))
            emit serverProcessError(res);
        else
        {
            QRegularExpression sitePattern(".*(http[s]?://localhost:[0-9]{1,5}/?).*");
            QRegularExpressionMatch match = sitePattern.match(res);
            if(match.hasMatch())
            {
                res = match.capturedTexts().at(1);
                QUrl url(res);
                if(url.isValid())
                    emit serverLaunched(res);
             }
        }
    });

    connect(m_serverProcess, &QProcess::readyReadStandardError, this, [this] {
        QString res(m_serverProcess->readAll());
        emit serverProcessError(res);
    });
}

void HPEHexoController::checkHexoInstallation()
{
    if(m_workingDir == QDir())
    { m_hexoInstalled = false; return; }

    m_processingDialog->setPrompt(tr("Checking Hexo Installation"));
    m_commmadProcess->setArguments(HEXO_CHECK);
    disconnect(m_commmadProcess, nullptr, this, nullptr);  //remove previous listeners
    connect(m_commmadProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError e) {
        if(e == QProcess::FailedToStart)
        {
            m_hexoInstalled = false;
            emit npxNotFound("Error: npx not found");
        }
        else
        {
            m_hexoInstalled = false;
            emit unknownError("Error: Unknown error!!");
        }
    });
    connect(m_commmadProcess, &QProcess::finished, this, [this] {
        QString res = QString(m_commmadProcess->readAllStandardOutput());
        QString error = QString(m_commmadProcess->readAllStandardError());
        if(!error.isEmpty())
        { m_hexoInstalled = false; emit hexoNotFound("Error" + error); }
        else if(res.contains("not found"))
        { m_hexoInstalled = false; emit hexoNotFound("Not Found Error" + res);  }
        else if(res.contains("hexo <command>"))
        { m_hexoInstalled = true;  emit hexoEnvironmentChecked(true);  }
        else if(res.isEmpty())
        { m_hexoInstalled = false; emit hexoNotFound("Error this is not a Hexo projetc dir"); }
        else
        { m_hexoInstalled = false; emit unknownError("Unknown Error" + res);  }
    });
    m_commmadProcess->start(QProcess::ReadOnly);
    emit commandProcessStart();
}

bool HPEHexoController::isCommandProcessAvailable() const
{
    return (m_hexoInstalled && (m_commmadProcess->state() == QProcess::NotRunning));
}

bool HPEHexoController::processCommand(HPEHexoController::COMMAND_TYPE type)
{
    if(!isCommandProcessAvailable())
        return false;

    //special configuring
    switch (type)
    {
    case CREATE:
    case SERVER:
        return false;
    default:
        m_commmadProcess->setArguments(HEXO_ARGUMENTS.value(type));
        break;
    }

    disconnect(m_commmadProcess, nullptr, this, nullptr);  //remove previous listeners
    connect(m_commmadProcess, &QProcess::finished, this, [this, type] {
        QString res(m_commmadProcess->readAllStandardOutput());
        QString error(m_commmadProcess->readAllStandardError());

        //ADD SETTING: strict error processing
        if(!error.isEmpty() && (error.contains("FATAL", Qt::CaseInsensitive) ||
                                error.contains("ERROR", Qt::CaseInsensitive)))
            emit commandProcessError(error);
        else if(res.contains("FATAL", Qt::CaseInsensitive) ||
                res.contains("ERROR", Qt::CaseInsensitive))
            emit commandProcessError(res);
        else
            emit processFinished(res, type);
    });

    m_commmadProcess->start(QProcess::ReadOnly);
    emit commandProcessStart();
    return true;
}

