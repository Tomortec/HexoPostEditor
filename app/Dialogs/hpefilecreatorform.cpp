/**
 * @file hpefilecreatorform.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpefilecreatorform.h"
#include "ui_hpefilecreatorform.h"

#include <QCompleter>
#include <QRegularExpression>

#include "Controller/hpehexocontroller.h"


HPEFileCreatorForm::HPEFileCreatorForm(HPEHexoController* hexoController, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPEFileCreatorForm),
    m_hexoController(hexoController)
{
    ui->setupUi(this);

    QCursor pointerCursor(Qt::PointingHandCursor);
    ui->backButton->setCursor(pointerCursor);
    ui->confirmButton->setCursor(pointerCursor);

    ui->layoutSelector->addItems(HPEFileCreatorForm::LAYOUT_TYPES);
    ui->layoutSelector->lineEdit()->setPlaceholderText("Layout");
    QCompleter* completer = new QCompleter(HPEFileCreatorForm::LAYOUT_TYPES, ui->layoutSelector);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->layoutSelector->setCompleter(completer);

    connect(ui->backButton, &QPushButton::clicked, this, [this]{ emit this->back(); });
    connect(ui->confirmButton, &QPushButton::clicked, this, &HPEFileCreatorForm::createNewFile);
    connect(m_hexoController, &HPEHexoController::processFinished, this, [this](const QString& res, HPEHexoController::COMMAND_TYPE type){
       if(type == HPEHexoController::CREATE)
       {
            QRegularExpressionMatch match = QRegularExpression("Created: (.+)source/(.+)$").match(res);
            if(match.hasMatch())
            {
                //TODO: store the layout user selected
                m_newFile = match.capturedTexts().at(2);
                emit fileCreated(m_targetDir.absoluteFilePath(m_newFile));
            }
       }
    });
    connect(ui->titleEdit, &QLineEdit::textChanged, ui->confirmButton, [this]{
        ui->confirmButton->setEnabled(!ui->titleEdit->text().isEmpty());
    });
}

HPEFileCreatorForm::~HPEFileCreatorForm()
{
    delete ui;
}

void HPEFileCreatorForm::setDir(const QDir &targetDir)
{
    m_targetDir = targetDir;
    ui->currentDirLabel->setText(tr("Current Dir: ") + m_targetDir.absolutePath());
    ui->currentDirLabel->setWordWrap(true);
    //should not call HPEHexoController::setDir
}

void HPEFileCreatorForm::reset()
{
    ui->titleEdit->clear();
}

void HPEFileCreatorForm::createNewFile()
{
    if(m_hexoController)
    {
        m_hexoController->createPost(ui->titleEdit->text(),
            HPEFileCreatorForm::LAYOUT_ARGUMENTS.value(ui->layoutSelector->currentText(),
                ui->layoutSelector->currentText()));    //if user inputs other strings, regard it as customized layout
    }
}

