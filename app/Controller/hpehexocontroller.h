/**
 * @file hpehexocontroller.h
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-22
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEHEXOCONTROLLER_H
#define HPEHEXOCONTROLLER_H

#include <QDir>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>

class QTerminalProcess;
class HPEProcessingDialog;

/**
 * @class HPEHexoController
 * @brief This class is used to process Hexo commands
 * @since 1.0.0
 * 
 * @ingroup controller
 * 
 * An HPEHexoController will provide two QProcess,
 * one for running 'normal' Hexo commands, which will not start a loop, 
 * such as 'new', 'generate', 'clean' and 'deploy',
 * the other is used to run Hexo local server.
 * 
 * @par About NPX
 * 
 * @note All Hexo commands is executed based on NPX.
 * 
 * To find the path of NPX, HPEHexoController needs a QTerminalProcess
 * who provides executables' paths in system environment.
 * 
 * @par Running commands
 * 
 * Before running commands, checkHexoInstallation() is needed 
 * to ensure Hexo installation environment. This method will set m_hexoInstalled
 * 
 * To set working directory, call setDir() which will 
 * call checkHexoInstallation() auto-ly.
 * 
 * For 'normal' commands, HPEHexoController provides 
 * createPost(), generate(), clean() and deploy() slots to execute Hexo commands.
 * These commands will be executed by m_commandProcess.
 * 
 * For server commands, HPEHexoController provides 
 * launchServer() and stopServer() to start and end Hexo server.
 * These commands will be executed by m_serverProcess.
 * 
 * @par Processing Dialog
 * 
 * HPEHexoController will show an HPEProcessingDialog to tell the command being executed by m_commandProcess
 * and warn if error occurs.
 * 
 * @par Commands Process Signals
 * 
 * Once the m_commandProcess finishes configuring and calls QProcess::start(),
 * HPEHexoController will emit commandProcessStart(). (That is, this signal can be triggered in
 * checkHexoInstallation(), createPost(), generate(), clean() and deploy())
 * 
 * If checkHexoInstallation() check the environment without error,
 * HPEHexoController will emit hexoEnvironmentChecked() signal.
 * Otherwise, npxNotFound(), hexoNotFound() or unknownError() will be emitted to warn and transfer error desciption.
 * 
 * If executing 'normal' commands occurs error, 
 * HPEHexoController will emit commandProcessError() signal to warn and transfer error desciption.
 * 
 * After executing 'normal' commands successfully,
 * HPEHexoController will emit processFinished() signal to 
 * tell the result message the type of command being executed.
 * 
 * @par Server Process Signals
 * 
 * Once the m_serverProcess starts Hexo server successfully,
 * HPEHexoController will emit serverLaunched() signal which transfers the local domain server running on.
 * 
 * If error occurs in m_serverProcess, serverProcessError() will be emitted.
 * 
 * @see QProcess, QTerminalProcess
*/
class HPEHexoController : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Construct a new HPEHexoController object with process and working directory
     * 
     * @param[in] process Needed, to find NPX's path
     * @param[in] targetDir Process working directory
     * @param[in] parent 
    */
    explicit HPEHexoController(QTerminalProcess* process,
                               const QDir& targetDir = QDir(),
                               QObject *parent = nullptr);

    /**
     * @brief Stop server and kill m_serverProcess and m_commandProcess
     * 
    */
    ~HPEHexoController();

public:

    /**
     * @brief List frequent Hexo commands
     * 
    */
    enum COMMAND_TYPE {
        CREATE, GENERATE, CLEAN,
        SERVER, DEPLOY
    };

private:

    /**
     * @brief Used to tell the command being executed by m_commandProcess
     * and warn if error occurs.
     * 
    */
    HPEProcessingDialog* m_processingDialog;

    /**
     * @brief Stores the working directory. This property can be set
     * only by setDir().
     * 
     * @attention This property can be set by setDir() only.
     * 
    */
    QDir m_workingDir;

    /**
     * @brief Used to execute CHECK, CREATE, GENERATE, CLEAN and DEPLOY commands.
     * 
    */
    QProcess* m_commmadProcess = nullptr;

    /**
     * @brief Used to load Hexo server
     * 
    */
    QProcess* m_serverProcess = nullptr;

    /**
     * @brief Holds whether Hexo is installed in current working directory.
     * This property can be set by checkHexoInstallation() only.
     * 
     * @attention This property can be set by checkHexoInstallation() only.
    */
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

    /**
     * @brief Set the working directory of the two processes 
     * and call checkHexoInstallation() to check Hexo installation in new dir.
     * 
    */
    void setDir(const QDir&);

public slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief This method will check Hexo installation
     * in current working dir (m_workingDir). This method is 
     * essential to any other Hexo command processing methods.
     * 
    */
    void checkHexoInstallation();

    /**
     * @brief Ask Hexo to create a new post with title and layout. ( ask Hexo to execute 'new' )
     * Note that if m_commandProcess is running, this method will return false.
     * 
     * @attention If m_commandProcess is running, this method will return false.
     * 
     * @param[in] title 
     * @param[in] layout Hexo post's layout
     * @return true if m_commandProcess has started. If m_commandProcess is running 
     * or error occurs, return false.
     * 
     * @see isCommandProcessAvailable()
     * 
     * @pre checkHexoInstallation() is called.
    */
    bool createPost(const QString& title, const QString& layout = QString(""));

    /**
     * @brief Call processCommand() to execute 'generate'.
     * Note that if m_commandProcess is running, this method will return false.
     * 
     * @attention If m_commandProcess is running, this method will return false.
     * 
     * @return true if m_commandProcess has started. If m_commandProcess is running 
     * or error occurs, return false.
     * 
     * @see processCommand(), isCommandProcessAvailable()
     * 
     * @pre checkHexoInstallation() is called.
    */
    bool generate();

    /**
     * @brief Call processCommand() to execute 'clean'.
     * Note that if m_commandProcess is running, this method will return false.
     * 
     * @attention If m_commandProcess is running, this method will return false.
     * 
     * @return true if m_commandProcess has started. If m_commandProcess is running 
     * or error occurs, return false.
     * 
     * @see processCommand(), isCommandProcessAvailable()
     * 
     * @pre checkHexoInstallation() is called.
    */
    bool clean();

    /**
     * @brief Call processCommand() to execute 'deploy'.
     * Note that if m_commandProcess is running, this method will return false.
     * 
     * @attention If m_commandProcess is running, this method will return false.
     * 
     * @return true if m_commandProcess has started. If m_commandProcess is running 
     * or error occurs, return false.
     * 
     * @see processCommand(), isCommandProcessAvailable()
     * 
     * @pre checkHexoInstallation() is called.
    */
    bool deploy();

    /**
     * @brief Ask Hexo to lauch server ( execute 'server' )
     * 
     * @attention If m_serverProcess is running, this method will return true.
     * 
     * @return true if m_serverProcess has started or is running. 
     * If error occurs, return false.
     * 
     * @pre checkHexoInstallation() is called.
    */
    bool launchServer();

    /**
     * @brief If m_serverProcess is running, stop it
     * by sending SIGINT signal.
     * 
    */
    void stopServer();
/**
 * @}
*/
private:

    /**
     * @brief Bind all frequent signals with HPEProcessingDialog
     * to update it.
     * 
    */
    void bindingProcessingDialogEvents();

    /**
     * @brief Connect functions with m_serverProcess's signals.
     * 
    */
    void bindingServerEvents();

    /**
     * @brief Check if m_commandProcess is available
     * 
     * @return true if Hexo has installed in m_workingDir and m_commandProcess is not running.
    */
    bool isCommandProcessAvailable() const;
    
    /**
     * @brief Execute Hexo command based on type
     * 
     * @param[in] type GENERATE, CLEAN or DEPLOY
     * @return true if m_commandProcess is available and the type is 
     * one of GENERATE, CLEAN and DEPLOY
    */
    bool processCommand(COMMAND_TYPE type);

signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when m_commandProcess 
     * has start(). It can be triggered in checkHexoInstallation(), createPost(), generate(), clean() and deploy()
     * 
    */
    void commandProcessStart();

    /**
     * @brief This signal is triggered when checkHexoInstallation()
     * got unknown error. This signal transfers error description.
     * 
    */
    void unknownError(const QString&);

    /**
     * @brief This signal is triggered when checkHexoInstallation()
     * cannot find NPX with NPX_PATH. This signal transfers error description.
     * 
    */
    void npxNotFound(const QString&);

    /**
     * @brief This signal is triggered when checkHexoInstallation()
     * cannot find Hexo by using NPX. This signal transfers error description.
     * 
    */
    void hexoNotFound(const QString&);

    /*void createError(const QString&);
    void generateError(const QString&);
    void cleanError(const QString&);
    void serverError(const QString&);
    void deployError(const QString&);*/

    /**
     * @brief This signal is triggered when error occurs during 
     * m_commandProcess's execution. This signal transfers error description.
     * 
    */
    void commandProcessError(const QString&);

    /**
     * @brief This signal is triggered when error occurs during 
     * m_serverProcess's execution. This signal transfers error description.
     * 
    */
    void serverProcessError(const QString&);

    /**
     * @brief This signal is triggered when m_serverProcess 
     * has started. This signal transfers the domain Hexo server runs on.
     * 
    */
    void serverLaunched(const QString&);

    /**
     * @brief This signal is triggered when checkHexoInstallation()
     * is done. The data it transfer is always true.
     * 
    */
    void hexoEnvironmentChecked(bool);

    /**
     * @brief This signal is triggered when processCommand() 
     * finishes. It will transfer full text got from Hexo and the type of command executed.
     * 
    */
    void processFinished(const QString&, HPEHexoController::COMMAND_TYPE);
/**
 * @}
*/
};

#endif // HPEHEXOCONTROLLER_H
