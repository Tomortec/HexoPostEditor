#include "qterminalwidget.h"

#include <csignal>
#include <QVBoxLayout>
#include <QHostInfo>
#include <QTextBlock>
#include <QKeyEvent>

#include "qterminalprocess.h"

QTerminalWidget::QTerminalWidget(QWidget *parent)
    : QTextEdit{parent}
{
    this->setPlaceholderText(tr("Haven't got a Hexo project directory"));
    this->setReadOnly(true);

    m_process = new QTerminalProcess(this);

    m_currentUser = qgetenv("USER");
    if(m_currentUser.isEmpty())
        m_currentUser = qgetenv("USERNAME");

    connect(this, &QTerminalWidget::commandEnter, m_process, &QTerminalProcess::onCommandEnter);
    connect(m_process, &QTerminalProcess::stateChanged, this, &QTerminalWidget::onProcessStateChanged);
    connect(m_process, &QTerminalProcess::readyReadStandardOutput, this, &QTerminalWidget::onReadyReadStandardOutput);
    connect(m_process, &QTerminalProcess::readyReadStandardError,  this, &QTerminalWidget::onReadyReadStandardError);
    connect(m_process, &QTerminalProcess::errorOccurred, this, &QTerminalWidget::onProcessErrorOccurred);
    connect(m_process, &QTerminalProcess::filteredCommandCompleted,this, &QTerminalWidget::onFilteredCommandCompleted);
    connect(m_process, &QTerminalProcess::workingDirChanged, this, &QTerminalWidget::onWorkingDirChanged);
    connect(m_process, &QTerminalProcess::commandNotFound, this, [this](const QString& command){
        this->outputError(QString("Command %1 not found!").arg(command));
        appendNewLine();
    });
}

QTerminalWidget::~QTerminalWidget()
{
    sendSIGINT();
    m_process->terminate();
    m_process->kill();
    m_process->deleteLater();
}

void QTerminalWidget::beautifyTerminal(const QFont &font,
                                 const QColor &backgroundColor,
                                 const QColor &foregroundColor,
                                 const QColor &selectionBackgroundColor,
                                 int cursorWidth)
{
    this->setFont(font);
    this->setCursorWidth(cursorWidth);
    this->setStyleSheet(QString("QTextEdit{ "
                                      "background-color: %1;"
                                      "selection-background-color: %2;"
                                      " }").arg(backgroundColor.name(),
                                                selectionBackgroundColor.name()));
    this->setTextColor(foregroundColor);
}

void QTerminalWidget::keyPressEvent(QKeyEvent *event)
{
    QTextCursor cursor = this->textCursor();
    if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_A)
    {
        cursor.setPosition(m_commandPrompt.length());
        this->setTextCursor(cursor);
        return;
    }
    else if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_C)
    {
        //emit SIGINT
        sendSIGINT();
        return;
    }
    else if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Z)
    {
        //emit SIGTSTP
        sendSIGTSTP();
        return;
    }
    else if(event->key() == Qt::Key_Home)
    {
        cursor.setPosition(m_commandPrompt.length());
        this->setTextCursor(cursor);
        return;
    }
    else if(event->key() == Qt::Key_Left)
    {
        if(cursor.positionInBlock() <= m_commandPrompt.length())
            return;
    }
    else if(event->key() == Qt::Key_Up)
    {
        if(m_priorCommandsIndex - 1 > 0)
        {
            --m_priorCommandsIndex;
            clearCommand();
            this->insertPlainText(m_priorCommands.at(m_priorCommandsIndex));
        }
        return;
    }
    else if(event->key() == Qt::Key_Down)
    {
        if(m_priorCommandsIndex + 1 < m_priorCommands.length())
        {
            ++m_priorCommandsIndex;
            clearCommand();
            this->insertPlainText(m_priorCommands.at(m_priorCommandsIndex));
        }
        else
            clearCommand();
        return;
    }
    else if(event->key() == Qt::Key_Backspace)
    {
        if(cursor.positionInBlock() <= m_commandPrompt.length())
            return;
    }
    else if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(!cursor.block().text().isEmpty())
        {
            QString command = cursor.block().text().sliced(m_commandPrompt.length())
                                .simplified(); // removes extra space
            if(!command.isEmpty())
            {
                m_priorCommands.append(command);
                m_priorCommandsIndex = m_priorCommands.length();
                this->setReadOnly(true);
                emit commandEnter(command);
            }
            else
                this->appendNewLine();
            return;
        }
    }
    QTextEdit::keyPressEvent(event);
}

void QTerminalWidget::mousePressEvent(QMouseEvent *)
{
    return;
}

void QTerminalWidget::setDir(const QDir &dir)
{
    m_currentDir = dir;
    assert(!m_currentUser.isEmpty());

    m_commandPrompt = QString("%1@%2 %3 > ").arg(m_currentUser,
                                                 QHostInfo::localHostName(),
                                                 m_currentDir.dirName());

    appendNewLine();
    m_process->setWorkingDirectory(m_currentDir.absolutePath());
    this->setReadOnly(false);
}

QTerminalProcess *QTerminalWidget::getProcess() const
{
    return m_process;
}

void QTerminalWidget::appendNewLine()
{
    this->append(m_commandPrompt);
    this->setReadOnly(false);
}

void QTerminalWidget::onReadyReadStandardOutput()
{
    this->append(m_process->readAllStandardOutput());
}

void QTerminalWidget::onReadyReadStandardError()
{
    this->outputError(m_process->readAllStandardError());
}

void QTerminalWidget::onProcessStateChanged(int state)
{
    switch(state)
    {
    case QTerminalProcess::NotRunning:
        appendNewLine();
        qDebug() << "Running stopped";
        break;
    case QTerminalProcess::Running:
        qDebug() << "Running";
        break;
    default:
        break;
    }
}

void QTerminalWidget::onFilteredCommandCompleted()
{
    appendNewLine();
}

void QTerminalWidget::onWorkingDirChanged(const QString &dir)
{
    if(!QDir(dir).exists())
    {
        outputError(QString("No such dir %1").arg(dir));
        appendNewLine();
    }
    else
        setDir(QDir(dir));
}

void QTerminalWidget::sendSIGINT()
{
    if(m_process->state() == QTerminalProcess::Running)
        kill(m_process->processId(), SIGINT);
}

void QTerminalWidget::sendSIGTSTP()
{
    if(m_process->state() == QTerminalProcess::Running)
        kill(m_process->processId(), SIGTSTP);
}

void QTerminalWidget::clearCommand()
{
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::BlockUnderCursor);
    cursor.removeSelectedText();
    appendNewLine();
}

void QTerminalWidget::outputError(const QString &error)
{
    QColor originalColor = this->textColor();
    this->setTextColor(Qt::red);
    this->append(error);
    this->setTextColor(originalColor);
}

void QTerminalWidget::onProcessErrorOccurred(int error)
{
    switch(error)
    {
    case QTerminalProcess::FailedToStart:
        this->outputError("Failed to start. Either the invoked program is missing, "
                          "or you may have insufficient permissions to invoke the program."
                          "Sometimes, you may use 'set [ENVIRONMENT_VARIABLE]=[value]' "
                          "to set your working environment.");
        break;
    case QTerminalProcess::Crashed:
        this->outputError(QString("%1 crashed.").arg(m_priorCommands.last()));
        break;
    default:
        this->outputError(QString("Unknown Error occurred when executing '%1'").arg(
                              m_priorCommands.last()));
        break;
    }
    appendNewLine();
}
