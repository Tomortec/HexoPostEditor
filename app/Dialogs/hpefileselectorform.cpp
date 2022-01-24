/**
 * @file hpefileselectorform.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpefileselectorform.h"
#include "ui_hpefileselectorform.h"

#include <QDirIterator>

HPEFileSelectorForm::HPEFileSelectorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPEFileSelectorForm)
{
    ui->setupUi(this);

    QCursor pointerCursor(Qt::PointingHandCursor);
    ui->backButton->setCursor(pointerCursor);
    ui->confirmButton->setCursor(pointerCursor);

    connect(ui->backButton, &QPushButton::clicked, this, [this]{ reset(); emit back(); });
    connect(ui->confirmButton, &QPushButton::clicked, this, [this]{
        emit confirm(m_targetDir.filePath(m_selectedFileName));
    });
    connect(ui->fileListWidget, &QListWidget::currentTextChanged, this, [this](const QString& current){
        m_selectedFileName = current;
    });
    connect(ui->fileListWidget, &QListWidget::itemSelectionChanged, this, [this]{
        if(!ui->fileListWidget->selectedItems().empty())
            ui->confirmButton->setEnabled(true);
    });
    connect(ui->fileListWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item){
        m_selectedFileName = item->text();
        emit confirm(m_targetDir.filePath(m_selectedFileName));
    });
}

HPEFileSelectorForm::~HPEFileSelectorForm()
{
    delete ui;
}

void HPEFileSelectorForm::setDir(const QDir &targetDir)
{
    m_targetDir = targetDir;

    ui->currentDirLabel->setText(tr("Current Dir: ") + m_targetDir.absolutePath());
    ui->currentDirLabel->setWordWrap(true);
    QDirIterator markdownIterator(targetDir.absolutePath(), {"*.md"}, QDir::Files | QDir::Writable, QDirIterator::Subdirectories);
    QStringList markdownFiles;
    while(markdownIterator.hasNext())
    {
        markdownIterator.next();
        markdownFiles.append(m_targetDir.relativeFilePath(markdownIterator.filePath()));
    }
    ui->fileListWidget->addItems(markdownFiles);
}

void HPEFileSelectorForm::reset()
{
    ui->confirmButton->setDisabled(true);
    ui->fileListWidget->clear();
}
