#ifndef HPEHEXOCONTROLLER_H
#define HPEHEXOCONTROLLER_H

#include <QDir>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>

class QTerminalProcess;
class HPEProcessingDialog;

class HPEHexoController : public QObject
{
    Q_OBJECT
public:
    explicit HPEHexoController(QTerminalProcess* process,
                               const QDir& targetDir = QDir(),
                               QObject *parent = nullptr);
    ~HPEHexoController();

public:
    enum COMMAND_TYPE {
        CREATE, GENERATE, CLEAN,
        SERVER, DEPLOY
    };

private:
    HPEProcessingDialog* m_processingDialog;
    QDir m_workingDir;
    QProcess* m_commmadProcess = nullptr;
    QProcess* m_serverProcess = nullptr;

    bool m_hexoInstalled = false;

    //use npx to search command in project or globally
    const QString NPX_PROGRAM   = "npx";
    QString NPX_PATH;

    //arguments
    const QStringList HEXO_CHECK    = {"--no-install", "hexo"};
    const QStringList HEXO_CREATE   = {"hexo", "new"};  //hexo new <title>
    const QStringList HEXO_GENERATE = {"hexo", "g"};
    const QStringList HEXO_CLEAN    = {"hexo", "clean"};
    const QStringList HEXO_SERVER   = {"hexo", "s"};
    const QStringList HEXO_DEPLOY   = {"hexo", "d"};

    const QMap<COMMAND_TYPE, QStringList> HEXO_ARGUMENTS = {
        {CREATE, HEXO_CREATE}, {GENERATE, HEXO_GENERATE},
        {CLEAN,  HEXO_CLEAN},  {SERVER,   HEXO_SERVER}, {DEPLOY, HEXO_DEPLOY}
    };

public:
    void setDir(const QDir&);

public slots:
    void checkHexoInstallation();

    /* if the process is processing, this methods below will return false */
    bool createPost(const QString& title, const QString& layout = QString(""));
    /* if the process is processing, this methods below will return false */
    bool generate();
    /* if the process is processing, this methods below will return false */
    bool clean();
    /* if the process is processing, this methods below will return false */
    bool launchServer();
    void stopServer();
    /* if the process is processing, this methods below will return false */
    bool deploy();

private:
    void bindingProcessingDialogEvents();
    void bindingServerEvents();

    //if hexo not found or the process is running, it return false
    bool isCommandProcessAvailable() const;
    //if hexo not found or the process is running, it return false
    bool processCommand(COMMAND_TYPE type);

signals:
    void commandProcessStart();

    void unknownError(const QString&);
    void npxNotFound(const QString&);
    void hexoNotFound(const QString&);

    /*void createError(const QString&);
    void generateError(const QString&);
    void cleanError(const QString&);
    void serverError(const QString&);
    void deployError(const QString&);*/
    void commandProcessError(const QString&);
    void serverProcessError(const QString&);

    void serverLaunched(const QString&);
    void hexoEnvironmentChecked(bool);
    void processFinished(const QString&, HPEHexoController::COMMAND_TYPE);
};

#endif // HPEHEXOCONTROLLER_H
