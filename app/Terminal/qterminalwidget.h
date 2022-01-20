#ifndef QTERMINALWIDGET_H
#define QTERMINALWIDGET_H

#include <QWidget>
#include <QDir>
#include <QTextEdit>
#include <QFontMetrics>

class QTerminalProcess;

class QTerminalWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit QTerminalWidget(QWidget *parent = nullptr);
    ~QTerminalWidget();

private:
    QString m_currentUser;
    QDir m_currentDir;

    QStringList m_priorCommands{ " " };
    int m_priorCommandsIndex = 0;

    QString m_commandPrompt;

    QTerminalProcess* m_process;

public:
    void beautifyTerminal(const QFont& font = QFont(),
                          const QColor& backgroundColor = Qt::black,
                          const QColor& foregroundColor = Qt::white,
                          const QColor& selectionBackgroundColor  = Qt::white,
                          int cursorWidth = 1/* = QFontMetrics::averageCharWidth()*/);

    void setDir(const QDir&);
    QTerminalProcess *getProcess() const;

protected:
    void keyPressEvent(QKeyEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

public slots:
    void appendNewLine();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onProcessStateChanged(int);
    void onFilteredCommandCompleted();
    void onWorkingDirChanged(const QString&);
    void sendSIGINT();
    void sendSIGTSTP();

private slots:
    void clearCommand();
    void outputError(const QString&);
    void onProcessErrorOccurred(int);

signals:
    void commandEnter(const QString&);
};

#endif // QTERMINALWIDGET_H
