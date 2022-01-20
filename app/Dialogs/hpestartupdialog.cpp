/**
 * @file hpestartupdialog.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpestartupdialog.h"
#include "ui_hpestartupdialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "hpesettings.h"
#include "hpehexocontroller.h"

#include "hpefileselectorform.h"
#include "hpefilecreatorform.h"

#include "Terminal/qterminalprocess.h"

HPEStartupDialog::HPEStartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HPEStartupDialog)
{
    ui->setupUi(this);

    this->resize(575, 375);
    this->setFixedSize(575, 375);
    setModal(true);
    setWindowFlags(Qt::Dialog);

    if(!HPESettings::config()->isDefaultValue("basic/presetDir"))
    {
        m_targetDir = QDir(HPESettings::config()->value("basic/presetDir",
                                                        QDir::homePath()).toString());
        ui->dirEdit->setText(m_targetDir.absolutePath());
    }

    m_commandFinder = new QTerminalProcess(this);
    m_dirValidator = new HPEHexoController(m_commandFinder, m_targetDir, this);

    m_fileSelectorForm = new HPEFileSelectorForm(this);
    m_fileSelectorForm->hide();
    this->layout()->addWidget(m_fileSelectorForm);

    m_fileCreatorForm = new HPEFileCreatorForm(m_dirValidator, this);
    m_fileCreatorForm->hide();
    this->layout()->addWidget(m_fileCreatorForm);

    connect(ui->dirSelector, &QPushButton::clicked, this, &HPEStartupDialog::onOpenDirDialog);
    connect(ui->dirEdit, &QLineEdit::textChanged, ui->bottomGroupBox, [this]{ ui->bottomGroupBox->setDisabled(true); });
    connect(m_dirValidator, &HPEHexoController::hexoEnvironmentChecked, this, [this]{
        ui->bottomGroupBox->setEnabled(true);
        HPESettings::config()->setValue("basic/presetDir", m_targetDir.absolutePath());
    });
    connect(ui->openFileButton, &QPushButton::clicked, this, &HPEStartupDialog::loadFileSelectorForm);
    connect(ui->newFileButton,  &QPushButton::clicked, this, &HPEStartupDialog::loadFileCreatorForm);

    connect(m_fileSelectorForm, &HPEFileSelectorForm::back, this, &HPEStartupDialog::backToDirSelectorWidget);
    connect(m_fileSelectorForm, &HPEFileSelectorForm::confirm, this, [this](const QString& res){
        m_targetFile = res;
        emit receiveTargetFilePath(res);
        this->accept();
    });

    connect(m_fileCreatorForm, &HPEFileCreatorForm::back, this, &HPEStartupDialog::backToDirSelectorWidget);
    connect(m_fileCreatorForm, &HPEFileCreatorForm::fileCreated, this, [this](const QString& res){
        m_targetFile = res;
        emit receiveTargetFilePath(res);
        this->accept();
    });

    if(!ui->dirEdit->text().isEmpty())  //if last opening dir is recorded
        m_dirValidator->checkHexoInstallation();
}

HPEStartupDialog::~HPEStartupDialog()
{
    delete ui;
}

QString HPEStartupDialog::getTargetFile() const
{
    return m_targetFile;
}

void HPEStartupDialog::onOpenDirDialog()
{
    m_targetDir = QFileDialog::getExistingDirectory(this, tr("select your Hexo project"),
                                                    HPESettings::config()->value("basic/presetDir").toString());
    if(m_targetDir.dirName() != "source" && !m_targetDir.cd("source"))
    {
        QMessageBox::warning(this, tr("Dir Error"), tr("Should be Hexo's 'source' folder!"));
        return;
    }
    ui->dirEdit->setText(m_targetDir.absolutePath());
    m_dirValidator->setDir(m_targetDir);
}

void HPEStartupDialog::loadFileSelectorForm()
{
    hideDirSelectorWidget();
    m_fileSelectorForm->setDir(m_targetDir);
    m_fileSelectorForm->show();
}

void HPEStartupDialog::loadFileCreatorForm()
{
    hideDirSelectorWidget();
    m_fileCreatorForm->setDir(m_targetDir);
    m_fileCreatorForm->show();
}

void HPEStartupDialog::backToDirSelectorWidget()
{
    m_fileSelectorForm->hide();
    m_fileCreatorForm->hide();
    ui->topGroupBox->show();
    ui->bottomGroupBox->show();
}

void HPEStartupDialog::hideDirSelectorWidget()
{
    ui->topGroupBox->hide();
    ui->bottomGroupBox->hide();
}
