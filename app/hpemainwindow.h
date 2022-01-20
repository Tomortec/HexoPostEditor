#ifndef HPEMAINWINDOW_H
#define HPEMAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QFontDialog>

#include "QsLog.h"
#include "QsLogDest.h"
#include "QsLogDestFile.h"

#include "hpehexocontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HPEMainWindow; }
QT_END_NAMESPACE

class HPEMarkdownEditor;
class HPEDocument;
class HPEDialog;
class HPEHexoController;
class HPEStartupDialog;
class QTerminalWidget;

class HPEMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    HPEMainWindow(QWidget *parent = nullptr);
    ~HPEMainWindow();

private:
    Ui::HPEMainWindow *ui;
    HPEStartupDialog* m_startupDialog = nullptr;
    HPEDocument* m_document = nullptr;
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

    HPEHexoController* m_hexoController = nullptr;

    QTerminalWidget* m_terminalWidget = nullptr;

public:
    HPEMarkdownEditor* getEditor() const;
    QString getFilePath() const;
    void openFile(const QString &path);

private:
    void loadQSSFile();
    void loadSettings();
    void bindingMenuEvents();
    void bindingEditorEvents();
    void bindingToolBarEvents();
    void bindingHexoControllerEvents();

    void synchronizeEditorScrollWithPage();

    bool isModified() const;
    void setWindowSizeState(const QString&);

    void createPostPreviewPlaceholder();

    void initLogger();
    void createLogPanel();
    void createTerminal();

private slots:

    void onLogUpdate(const QString&, int);

    //Menu -> File
    void onOpenStartupDialog();
    void onFileSave();
    void onFileSaveAs();

    //Menu -> Pattern
    void onOpenHeadingDialog();
    void onOpenLinkDialog();
    void onOpenImageDialog();
    void onOpenCodesDialog();
    void onOpenTableDialog();

    //Menu -> Hexo
    void onProcessFinished(const QString&, HPEHexoController::COMMAND_TYPE);
    void onServerLaunched(const QString&);

    //Menu -> Settings
    void onOpenFontsDialog();

    //Menu -> Window
    void onMaximize();

protected:
    void closeEvent(QCloseEvent*) override;

signals:
    void fileLoaded(const QString&);
};
#endif // HPEMAINWINDOW_H
