#include "qterminalprocess.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

QTerminalProcess::QTerminalProcess(QObject *parent)
    : QProcess(parent)
{
    this->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    searchExecutablePath();
}

QMap<QString, QString> QTerminalProcess::getExecutablePaths() const
{
    return m_executablePaths;
}

void QTerminalProcess::searchExecutablePath()
{
    QStringList paths = this->processEnvironment().value("PATH").split(QDir::listSeparator());
#ifdef Q_OS_MACOS
    QFile pathFile("/etc/paths");
    if(pathFile.open(QIODevice::ReadOnly))
        paths += QTextStream(&pathFile).readAll().split("\n");
#endif
    paths.removeDuplicates();
    for(const QString& path : paths)
    {
        QDirIterator iter(path, QDirIterator::Subdirectories);
        while(iter.hasNext())
        {
            QFileInfo file = QFileInfo(iter.next());
            if(file.isExecutable())
                m_executablePaths.insert(file.baseName(), file.absoluteFilePath());
        }
    }
}

QStringList QTerminalProcess::commandsFilter(const QString &command)
{
    QStringList splittedCommand = splitCommand(command);
    if(splittedCommand.first() == "cd")
    {
        if(splittedCommand.length() == 2)
        {
            QDir newDir = QDir(splittedCommand.at(1));
            if(newDir.isRelative())
                newDir = QDir(this->workingDirectory() + "/" + newDir.dirName());
            if(newDir.exists())
                this->setWorkingDirectory(newDir.absolutePath());
            emit workingDirChanged(newDir.absolutePath());
            //emit filteredCommandCompleted();
            splittedCommand.clear();
        }
    }
    else if(splittedCommand.first() == "set")
    {
        if(splittedCommand.length() == 2)
        {
            QStringList environmentVariable = splittedCommand.at(1).split("=");
            if(environmentVariable.length() == 2)
            {
                QString variable = QString(environmentVariable.at(1)).remove('"').remove("'")
                            .replace("$PATH", this->processEnvironment().value("PATH"));
                QString newEnvStr = processEnvironment().value(environmentVariable.at(0))
                            + QDir::listSeparator()
                            + variable;
                QProcessEnvironment newEnv = this->processEnvironment();
                newEnv.insert(environmentVariable.at(0), newEnvStr);
                this->setProcessEnvironment(newEnv);

                if(splittedCommand.first().compare("PATH", Qt::CaseInsensitive))
                {
                    searchExecutablePath();
                    emit executablePathsChanged();
                }

                splittedCommand.clear();
                emit filteredCommandCompleted();
            }
        }
    }
    return splittedCommand;
}

void QTerminalProcess::onCommandEnter(const QString &command)
{
    QStringList filteredCommand = commandsFilter(command);
    if(!filteredCommand.isEmpty())
    {
        if(m_executablePaths.contains(filteredCommand.first()))
        {
            this->setProgram(m_executablePaths.value(filteredCommand.first()));
            if(filteredCommand.length() > 1)
                this->setArguments(filteredCommand.sliced(1));
            this->start();
        }
        else
            emit commandNotFound(filteredCommand.first());
    }
}
