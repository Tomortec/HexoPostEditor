/**
 * @file hpemainwindow.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-23
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpemainwindow.h"
#include "ui_hpemainwindow.h"

#include "Controller/hpedocument.h"
#include "Controller/hpepreviewpage.h"
#include "Controller/hpesettings.h"
#include "Controller/hpehexocontroller.h"
#include "Controller/hpelocalresources.h"

#include "Editor/hpemarkdowneditor.h"
#include "Editor/hpeconvertedmarkdownpreview.h"

#include "Dialogs/hpedialog.h"
#include "Dialogs/hpestartupdialog.h"
#include "Dialogs/hpeaboutdialog.h"

#include "ThirdParty/Terminal/qterminalwidget.h"

HPEMainWindow::HPEMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HPEMainWindow)
{
    initLogger();

    ui->setupUi(this);

    loadQSSFile();
    loadSettings();

    //init pointers
    m_document = new HPEDocument();

    HPEPreviewPage* page = new HPEPreviewPage(this);
    ui->markdownPreview->setPage(page);
    ui->markdownPreview->setUrl(HPELocalResources::getLocalURLWithName("index.html"));

    //Listen to the changes in index.html
    //exposed to the JS side by QWebChannel under the name content.
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), m_document);
    page->setWebChannel(channel);

    createLogPanel();
    createTerminal();

    m_hexoController = new HPEHexoController(m_terminalWidget->getProcess(), QDir(""), this);

    //Action binding
    bindingMenuEvents();
    bindingEditorEvents();
    bindingToolBarEvents();
    bindingHexoControllerEvents();

    //after initialization
    createPostPreviewPlaceholder();

    QLOG_INFO() << "HexoPostEdtior launched : )";
    QLOG_INFO() << "Version: " << QCoreApplication::applicationVersion();
}

HPEMainWindow::~HPEMainWindow()
{
    QLOG_INFO() << "Quit HexoPostEditor";
    QsLogging::Logger::destroyInstance();
    delete ui;
}

HPEMarkdownEditor *HPEMainWindow::getEditor() const
{
    return ui->markdownField;
}

QString HPEMainWindow::getFilePath() const
{
    return m_filePath;
}

void HPEMainWindow::loadQSSFile()
{
    QFile file(":/resources/style.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
}

void HPEMainWindow::loadSettings()
{
    //load settings
    //load font
    QFont markdownFieldFont = QFont();
    markdownFieldFont.fromString(HPESettings::config()->value("markdownField/font").toString());
    ui->markdownField->setFont(markdownFieldFont);
    this->setFont(markdownFieldFont);
    //load window's size
    setWindowSizeState(HPESettings::config()->value("window/sizeState").toString());
    //load preview's size
    int previewWidth = HPESettings::config()->value("window/previewWidth").toInt();
    ui->splitter->setSizes(QList<int>{ui->splitter->geometry().width() - previewWidth,
                                      previewWidth});
}

void HPEMainWindow::bindingMenuEvents()
{
    connect(ui->actionMenuFont,      &QAction::triggered, this, &HPEMainWindow::onOpenFontsDialog);
    connect(ui->actionMenuOpenFile,  &QAction::triggered, this, &HPEMainWindow::onOpenStartupDialog);
    connect(ui->actionMenuNewFile,   &QAction::triggered, this, &HPEMainWindow::onOpenStartupDialog);
    connect(ui->actionMenuSave,      &QAction::triggered, this, &HPEMainWindow::onFileSave);
    connect(ui->actionMenuSaveAs,    &QAction::triggered, this, &HPEMainWindow::onFileSaveAs);
    connect(ui->actionMenuFullScreen,&QAction::triggered, this, &HPEMainWindow::onMaximize);
    connect(ui->actionMenuExit,      &QAction::triggered, this, &HPEMainWindow::close);

    connect(ui->actionMenuAbout,     &QAction::triggered, this, &HPEMainWindow::onOpenAboutDialog);
    connect(ui->actionMenuHexoHomepage, &QAction::triggered, this, [=]{
        QDesktopServices::openUrl(QUrl("https://hexo.io"));
    });
}

void HPEMainWindow::bindingEditorEvents()
{
    ui->convertedMarkdownPreview->connectEditor(ui->markdownField);
    connect(this, &HPEMainWindow::fileLoaded, ui->convertedMarkdownPreview, &HPEConvertedMarkdownPreview::filePathChanged);
    connect(ui->convertedMarkdownPreview, &HPEConvertedMarkdownPreview::convertedAll, this, [this](const QString& preview){
        m_document->setText(preview);
    });
    connect(ui->actionMenuStrong, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->wrapSelectionWithString("**"); });
    connect(ui->actionMenuItalic, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->wrapSelectionWithString("*"); });
    connect(ui->actionMenuUnderline, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->wrapSelectionWithString("<u>", "</u>"); });
    connect(ui->actionMenuStrike, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->wrapSelectionWithString("~~"); });
    connect(ui->actionMenuHeading1, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("# ", true); });
    connect(ui->actionMenuHeading2, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("## ", true); });
    connect(ui->actionMenuHeading3, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("### ", true); });
    connect(ui->actionMenuHeading4, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("#### ", true); });
    connect(ui->actionMenuHeading5, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("##### ", true); });
    connect(ui->actionMenuHeading6, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("###### ", true); });
    connect(ui->actionMenuHorizontalRule, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("***", true); });
    connect(ui->actionMenuQuote, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString(">"); });
    connect(ui->actionMenuUnorderedList, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("+ "); });
    connect(ui->actionMenuOrderedList, &QAction::triggered, ui->markdownField,
            [this]() { ui->markdownField->insertString("1. "); });  //TODO: not only "1"

    connect(ui->actionMenuLink,  &QAction::triggered, this, &HPEMainWindow::onOpenLinkDialog);
    connect(ui->actionMenuImage, &QAction::triggered, this, &HPEMainWindow::onOpenImageDialog);
    connect(ui->actionMenuCodes, &QAction::triggered, this, &HPEMainWindow::onOpenCodesDialog);
    connect(ui->actionMenuTable, &QAction::triggered, this, &HPEMainWindow::onOpenTableDialog);

    synchronizeEditorScrollWithPage();
}

void HPEMainWindow::bindingToolBarEvents()
{
    connect(ui->buttonMDHeading,&QPushButton::clicked, this, &HPEMainWindow::onOpenHeadingDialog);
    connect(ui->buttonMDBold,   &QPushButton::clicked, ui->actionMenuStrong, &QAction::trigger);
    connect(ui->buttonMDItalic, &QPushButton::clicked, ui->actionMenuItalic, &QAction::trigger);
    connect(ui->buttonMDUnderline, &QPushButton::clicked, ui->actionMenuUnderline, &QAction::trigger);
    connect(ui->buttonMDStrike, &QPushButton::clicked, ui->actionMenuStrike, &QAction::trigger);
    connect(ui->buttonMDLink,   &QPushButton::clicked, ui->actionMenuLink,   &QAction::trigger);
    connect(ui->buttonMDImage,  &QPushButton::clicked, ui->actionMenuImage,  &QAction::trigger);
    connect(ui->buttonMDCode,   &QPushButton::clicked, ui->actionMenuCodes,  &QAction::trigger);
    connect(ui->buttonMDList,   &QPushButton::clicked, ui->actionMenuUnorderedList,   &QAction::trigger);
    connect(ui->buttonHexoGenerate, &QPushButton::clicked, ui->actionMenuHexoGenerate, &QAction::trigger);
    connect(ui->buttonHexoServer,   &QPushButton::clicked, ui->actionMenuHexoServer,   &QAction::trigger);
    connect(ui->buttonHexoDeploy,   &QPushButton::clicked, ui->actionMenuHexoDeploy,   &QAction::trigger);

}

void HPEMainWindow::bindingHexoControllerEvents()
{
    connect(ui->actionMenuHexoGenerate, &QAction::triggered, this, &HPEMainWindow::onFileSave);
    connect(ui->actionMenuHexoGenerate, &QAction::triggered, m_hexoController, &HPEHexoController::generate);
    connect(ui->actionMenuHexoClean,    &QAction::triggered, m_hexoController, &HPEHexoController::clean);
    connect(ui->actionMenuHexoServer,   &QAction::triggered, m_hexoController, &HPEHexoController::launchServer);
    connect(ui->actionMenuHexoDeploy,   &QAction::triggered, m_hexoController, &HPEHexoController::deploy);
    connect(m_hexoController, &HPEHexoController::serverLaunched, this, &HPEMainWindow::onServerLaunched);
    connect(m_hexoController, &HPEHexoController::processFinished, this, &HPEMainWindow::onProcessFinished);
}

void HPEMainWindow::synchronizeEditorScrollWithPage()
{
    connect(ui->markdownField, &HPEMarkdownEditor::scrollVerticallyBy, this, [this](double d){
        double delta = d;
#ifdef Q_OS_MACOS
        //on macOS, the delta hasgot inverted
        delta *= -1.0;
#endif

        ui->markdownPreview->page()->runJavaScript(
                    QString("window.scrollBy({left:0,top: %1 *document.body.clientHeight,behavior: 'smooth'})")
                    .arg(delta / ui->markdownField->geometry().height()));
        //it's weird to scroll server page syncly
        //ui->postPreview->page()->runJavaScript(
        //            QString("window.scrollBy({left:0,top: %1 *document.body.clientHeight,behavior: 'smooth'})")
        //            .arg(double(delta) / ui->markdownField->geometry().height()));
    });
}

bool HPEMainWindow::isModified() const
{
    return ui->markdownField->document()->isModified();
}

void HPEMainWindow::openFile(const QString &path)
{
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_filePath = path;

    if(m_fileDir != QDir(QFileInfo(m_filePath).absoluteDir()))
        //before opening a new file, call stopServer
        m_hexoController->stopServer();


    m_fileDir = QDir(QFileInfo(m_filePath).absoluteDir());

    m_hexoController->setDir(m_fileDir);
    ui->markdownField->setPlainText(f.readAll());
    QLOG_INFO() << QString("File %1 loaded").arg(m_filePath);
    emit fileLoaded(m_filePath);
}

void HPEMainWindow::setWindowSizeState(const QString &state)
{
    if(state == "NORMAL")
        this->showNormal();
    else if(state == "FULLSCREEN")
        this->showFullScreen();
    else
        this->showMaximized();
}

void HPEMainWindow::createPostPreviewPlaceholder()
{
    if(!ui->postPreview->page())
    {
        HPEPreviewPage* page = new HPEPreviewPage(this);
        ui->postPreview->setPage(page);
    }
    ui->postPreview->page()->setHtml(tr(
        "<div style='margin:auto;padding-top:30%;font-size:3rem;width:40%;'>"
        "You should <b>launch Hexo server</b> first"
        "</div>"
    ));
}

void HPEMainWindow::initLogger()
{
    QsLogging::Logger::instance().addDestination(
        QsLogging::DestinationFactory::MakeFunctorDestination(
            this, SLOT(onLogUpdate(QString, int))
        )
     );
}

void HPEMainWindow::createLogPanel()
{
    QPlainTextEdit* logPanel = new QPlainTextEdit(ui->loggerPage);
    logPanel->setReadOnly(true);
    logPanel->setFrameShape(QFrame::NoFrame);
    if(!ui->loggerPage->layout())
    {
        QVBoxLayout* layout = new QVBoxLayout(ui->loggerPage);
        ui->loggerPage->setLayout(layout);
    }
    ui->loggerPage->layout()->addWidget(logPanel);
}

void HPEMainWindow::createTerminal()
{
    if(m_terminalWidget)
        return;

    m_terminalWidget = new QTerminalWidget(ui->terminalPage);
    m_terminalWidget->beautifyTerminal(this->font(), Qt::black, Qt::white, Qt::white,
                                       QFontMetrics(this->font()).averageCharWidth());
    m_terminalWidget->setDir(m_fileDir);
    if(!ui->terminalPage->layout())
    {
        QVBoxLayout* layout = new QVBoxLayout(ui->terminalPage);
        ui->terminalPage->setLayout(layout);
    }
    ui->terminalPage->layout()->addWidget(m_terminalWidget);
}

void HPEMainWindow::onLogUpdate(const QString &msg, int level)
{
    QPlainTextEdit* logPanel = ui->loggerPage->findChild<QPlainTextEdit*>();
    if(logPanel)
    {
        if(level <= QsLogging::InfoLevel)
            logPanel->appendPlainText(msg);
        else
            logPanel->appendHtml(QString("<h3 style='color:crimson;'>%1</h3>").arg(msg));
    }
}

void HPEMainWindow::onOpenStartupDialog()
{
    if (isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }

    if(!m_startupDialog)
    {
        m_startupDialog = new HPEStartupDialog(this);

        connect(m_startupDialog, &HPEStartupDialog::receiveTargetFilePath,
                this, [this](const QString& path){ openFile(path); });
    }
    m_startupDialog->exec();
}

void HPEMainWindow::onFileSave()
{
    if (m_filePath.isEmpty())
    {
        onFileSaveAs();
        return;
    }

    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                             QDir::toNativeSeparators(m_filePath), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << ui->markdownField->toPlainText();

    ui->markdownField->document()->setModified(false);
    QLOG_INFO() << QString("File %1 saved").arg(m_filePath);
}

void HPEMainWindow::onFileSaveAs()
{
    QFileDialog dialog(this, tr("Save MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("md");
    if (dialog.exec() != QDialog::Accepted)
        return;

    m_filePath = dialog.selectedFiles().constFirst();
    onFileSave();
}

void HPEMainWindow::onOpenHeadingDialog()
{
    if(m_headingDialog == nullptr)
    {
        m_headingDialog = new HPEDialog(HPEDialog::HEADING, this);
        connect(m_headingDialog, &HPEDialog::dialogConfirm, ui->markdownField,
                [this](const QString &res){ this->ui->markdownField->textCursor().insertText(res); });
    }
    m_headingDialog->exec();
}

void HPEMainWindow::onOpenLinkDialog()
{
    if(m_linkDialog == nullptr)
    {
        m_linkDialog = new HPEDialog(HPEDialog::LINK, this);
        connect(m_linkDialog, &HPEDialog::dialogConfirm, ui->markdownField,
                [this](const QString &res){ this->ui->markdownField->textCursor().insertText(res); });
    }
    m_linkDialog->exec();
}

void HPEMainWindow::onOpenImageDialog()
{
    if(m_imageDialog == nullptr)
    {
        m_imageDialog = new HPEDialog(HPEDialog::IMAGE, this);
        connect(m_imageDialog, &HPEDialog::dialogConfirm, ui->markdownField,
                [this](const QString &res){ this->ui->markdownField->textCursor().insertText(res); });
    }
    m_imageDialog->exec();
}

void HPEMainWindow::onOpenCodesDialog()
{
    if(m_codeDialog == nullptr)
    {
        m_codeDialog = new HPEDialog(HPEDialog::CODE, this);
        connect(m_codeDialog, &HPEDialog::dialogConfirm, ui->markdownField,
                [this](const QString &res){ this->ui->markdownField->textCursor().insertText(res); });
    }
    m_codeDialog->exec();
}

void HPEMainWindow::onOpenTableDialog()
{
    if(m_tableDialog == nullptr)
    {
        m_tableDialog = new HPEDialog(HPEDialog::TABLE, this);
        connect(m_tableDialog, &HPEDialog::dialogConfirm, ui->markdownField,
                [this](const QString &res){ this->ui->markdownField->textCursor().insertText(res); });
    }
    m_tableDialog->exec();
}

void HPEMainWindow::onOpenAboutDialog()
{
    if(m_aboutDialog == nullptr)
        m_aboutDialog = new HPEAboutDialog(this);
    m_aboutDialog->exec();
}

void HPEMainWindow::onProcessFinished(const QString &/*res*/, HPEHexoController::COMMAND_TYPE type)
{
    switch(type)
    {
    case HPEHexoController::COMMAND_TYPE::GENERATE:
    {
        if(ui->postPreview->page() && ui->postPreview->url().isValid())
            ui->postPreview->reload();
        break;
    }
    default:
        break;
    }
}

void HPEMainWindow::onServerLaunched(const QString &res)
{
    QUrl url(res);
    if(url.isValid())
    {
        if(!ui->postPreview->page())
        {
            HPEPreviewPage* page = new HPEPreviewPage(this);
            ui->postPreview->setPage(page);
        }
        ui->postPreview->setUrl(url);
        ui->tabWidget->setCurrentWidget(ui->postPreview);
    }
    else
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot get server's URL!"));
}

void HPEMainWindow::onOpenFontsDialog()
{
    QFont font = QFontDialog::getFont(0, ui->markdownField->font(), this);
    ui->markdownField->setFont(font);
    HPESettings::config()->setValue("markdownField/font", font.toString());
}

void HPEMainWindow::onMaximize()
{
    if(isFullScreen())
        showNormal(), HPESettings::config()->setValue("window/sizeState", "NORMAL");
    else if(isMaximized())
        showFullScreen(), HPESettings::config()->setValue("window/sizeState", "FULLSCREEN");
    else    //showing normal or minimized
        showMaximized(), HPESettings::config()->setValue("window/sizeState", "MAXIMIZED");
}

void HPEMainWindow::closeEvent(QCloseEvent *event)
{
    m_hexoController->stopServer();

    if(isModified() && ui->markdownField->toPlainText() != "")
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if(button != QMessageBox::Yes)
            event->ignore();
    }

    HPESettings::config()->setValue("window/previewWidth", ui->splitter->sizes().at(1));
}
