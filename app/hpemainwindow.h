/**
 * @file hpemainwindow.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-23
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEMAINWINDOW_H
#define HPEMAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QFontDialog>

#include "QsLog.h"
#include "QsLogDest.h"
#include "QsLogDestFile.h"

#include "Controller/hpehexocontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HPEMainWindow; }
QT_END_NAMESPACE

class HPEMarkdownEditor;
class HPEDocument;
class HPEDialog;
class HPEAboutDialog;
class HPEHexoController;
class HPEStartupDialog;
class QTerminalWidget;

/**
 * @class HPEMainWindow
 * @brief The main window of Hexo Post Editor
 * @since 1.0.0
 * 
 * @ingroup widgets
 * 
 * @par UI
 * 
 * HPEMainWindow has 1 HPEMarkdownEditor(ui->markdownField) for input,
 * 1 QWebEngineView(ui->markdownPreview) for viewing rendered Markdown,
 * 1 QWebEngineView(ui->postPreview) for viewing Hexo posts' preview page (the one running by Hexo's local server),
 * 1 HPEMarkdownEditor(ui->convertedMarkdownPreview) for viewing the source of Markdown (converted from Hexo's post),
 * 1 QTerminalWidget*(m_terminalWidget),
 * 1 ui->loggerPage for showing runtime logs.
 * 
 * @note * Actually, using LXQt's QTermWidget(https://github.com/lxqt/qtermwidget) which is much more stable is better,
 * however, at the time we are developing, QTermWidget does not support Qt6 :-(
 * 
 * @par Markdown Preview Working Process
 * 
 * Once the document in ui->markdownField(HPEMarkdownEditor) is changed, 
 * ui->convertedMarkdownPreview(HPEConvertedMarkdownPreview) will get and convert the 
 * changed document. After converting, ui->convertedMarkdownPreview will 
 * provide the converted standard Markdown for m_document, who will transfer the converted
 * document to ui->markdownPreview's page to render.
 * 
 * @par Hexo Command Executing Process
 * 
 * Hexo commands are executed by m_hexoController.
 * 
 * @see HPEHexoController
 * 
 * @par Exposure
 * 
 * HPEMainWindow exposes ui->markdownField and the path of current file
 * by getEditor() and getFilePath().
 * 
 * It also exposes openFile() method to load file.
*/
class HPEMainWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * @brief Construct a new HPEMainWindow object with parent
     * 
     * @param[in] parent 
    */
    HPEMainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroy logger and HPEMainWindow UI
     * 
    */
    ~HPEMainWindow();

private:
    Ui::HPEMainWindow *ui;

    /**
     * @brief An HPEStartupDialog used to guide user create or open a file
     * 
    */
    HPEStartupDialog* m_startupDialog = nullptr;

    /**
     * @brief Used to transfer data to web page
     * 
     * @see HPEDocument, QWebChannel
    */
    HPEDocument* m_document = nullptr;

    /**
     * @brief The absolute path of current file
     * 
    */
    QString m_filePath;
    QDir    m_fileDir;

    /* here is a bug that when the members are not set to nullptr initially,
     * one of the pointers below will NOT be a nullptr or NULL as expected,
     * making it jump through the condition check and resulting a CRASH.
    */
    HPEDialog* m_headingDialog = nullptr;
    HPEDialog* m_linkDialog    = nullptr;
    HPEDialog* m_imageDialog   = nullptr;
    HPEDialog* m_codeDialog    = nullptr;
    HPEDialog* m_tableDialog   = nullptr;
    HPEAboutDialog* m_aboutDialog = nullptr;

    HPEHexoController* m_hexoController = nullptr;

    QTerminalWidget* m_terminalWidget = nullptr;

public:

    /**
     * @brief Return the current HPEMarkdownEditor
     * 
     * @return Current HPEMarkdownEditor
    */
    HPEMarkdownEditor* getEditor() const;

    /**
     * @brief Return the absolute path of the file loaded
     * 
     * @return The absolute path of the file loaded
    */
    QString getFilePath() const;

    /**
     * @brief Open a new file with path.
     * This method will stop Hexo server if the Hexo directory has changed.
     * After loading the new file, fileLoaded() signal will be emitted.
     * 
     * @param[in] path The absolute path of a Markdown file.
    */
    void openFile(const QString &path);

protected:

    /**
     * @brief Overrides QMainWindow::closeEvent() to 
     * stop Hexo server and store settings via HPESettings
     * 
     * @see HPEHexoController, HPESetting
    */
    void closeEvent(QCloseEvent*) override;

private:

    /**
     * @brief Load QStyleSheet file
     * 
    */
    void loadQSSFile();

    /**
     * @brief Load stored settings
     * 
    */
    void loadSettings();

    /**
     * @brief Connect relevant signals and slots for menu bar
     * 
    */
    void bindingMenuEvents();

    /**
     * @brief Connect relevant signals and slots for editor
     * 
    */
    void bindingEditorEvents();

    /**
     * @brief Connect relevant signals and slots for tool bar
     * 
    */
    void bindingToolBarEvents();

    /**
     * @brief Connect relevant signals and slots for Hexo controller
     * 
    */
    void bindingHexoControllerEvents();

    /**
     * @brief Synchronize preview's scroll with editor
     * 
    */
    void synchronizeEditorScrollWithPage();

    /**
     * @brief Return whether the file opened is modified.
     * 
     * @return true the current file got modified
    */
    bool isModified() const;

    /**
     * @brief Set the Window size 
     * 
     * @param[in] state "NORMAL", "FULLSCREEN", "MAXIMIZED"
    */
    void setWindowSizeState(const QString& state);

    /**
     * @brief Create placeholder for ui->postPreview
     * 
    */
    void createPostPreviewPlaceholder();

    /**
     * @brief Connect QsLogging::Logger with onLogUpdate()
     * 
     * @see onLogUpdate()
    */
    void initLogger();

    /**
     * @brief Create a QPlainTextEdit in ui->loggerPage
     * to show logs
     * 
    */
    void createLogPanel();

    /**
     * @brief Create a QTerminalWidget in ui->terminalPage
     * 
    */
    void createTerminal();

private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Executed when QsLogging::Logger's log changes.
     * This slot will update the text in ui->loggerPage
     * 
    */
    void onLogUpdate(const QString&, int);

    //Menu -> File

    /**
     * @brief Executed when ui->actionMenuOpenFile or ui->actionMenuNewFile is triggered.
     * This slot will create an HPEStartupDialog (if needed) and show it.
     * 
     * @see HPEStartupDialog
    */
    void onOpenStartupDialog();

    /**
     * @brief Executed when ui->actionMenuSave is triggered or
     * ui->actionMenuHexoGenerate (user ask Hexo to 'generate').
     * This slot will save the file to current file path (m_filePath).
     * If current file path is empty, call onFileSaveAs() methd.
     * 
     * @see onFileSaveAs()
    */
    void onFileSave();

    /**
     * @brief Open a file dialog to the target path to save Markdown file
     * and call onFileSave() to save file.
     * 
    */
    void onFileSaveAs();

    //Menu -> Pattern

    /**
     * @brief Create (if needed) and show an HPEDialog with
     * HPEHeadingDialogForm.
     * 
    */
    void onOpenHeadingDialog();

    /**
     * @brief Create (if needed) and show an HPEDialog with
     * HPELinkDialogForm.
     * 
    */
    void onOpenLinkDialog();

    /**
     * @brief Create (if needed) and show an HPEDialog with
     * HPEImageDialogForm.
     * 
    */
    void onOpenImageDialog();

    /**
     * @brief Create (if needed) and show an HPEDialog with
     * HPECodesDialogForm.
     * 
    */
    void onOpenCodesDialog();

    /**
     * @brief Create (if needed) and show an HPEDialog with
     * HPETableDialogForm.
     * 
    */
    void onOpenTableDialog();

    void onOpenAboutDialog();

    //Menu -> Hexo

    /**
     * @brief Executed when m_hexoController finishes processing command.
     * 
     * @param[in] res
     * @param[in] type  If the type is GENERATE, ask ui->postPreview to reload
    */
    void onProcessFinished(const QString& /*res*/, HPEHexoController::COMMAND_TYPE type);

    /**
     * @brief If the url is valid, ask ui->postPreview
     * to load Hexo preview page.
     * 
     * @param[in] res The URL of Hexo local server
    */
    void onServerLaunched(const QString& res);

    //Menu -> Settings
    /**
     * @brief Open a QFontDialog to set the font
     * of ui->markdownField
     * 
    */
    void onOpenFontsDialog();

    //Menu -> Window
    /**
     * @brief Switch main window's size between 
     * normal, fullscreen and maximized and save the current size flag
     * 
    */
    void onMaximize();

/**
 * @}
*/
signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when new file is loaded to ui->markdownField.
     * 
     * @see openFile()
    */
    void fileLoaded(const QString&);
/**
 * @}
*/
};
#endif // HPEMAINWINDOW_H
