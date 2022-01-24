/**
 * @file hpeprocessingdialog.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpeprocessingdialog.h"
#include "ui_hpeprocessingdialog.h"

#include <QScreen>
#include <QTimer>
#include <QMovie>

HPEProcessingDialog::HPEProcessingDialog(const QString &prompt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HPEProcessingDialog)
{
    ui->setupUi(this);
    ui->closeButton->setVisible(false);

    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);

    if(!prompt.isEmpty())
        ui->prompt->setText(prompt);

    m_movie = new QMovie(":/animation/resources/anim_processing.gif");
    m_movie->setParent(ui->processingMovie);
    ui->processingMovie->setMovie(m_movie);
    m_movie->start();
}

HPEProcessingDialog::~HPEProcessingDialog()
{
    delete ui;
}

void HPEProcessingDialog::setPrompt(const QString &prompt)
{
    ui->prompt->setText(prompt);
}

void HPEProcessingDialog::moveToCenter()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height() - 100) / 2;
    move(x, y);
}

void HPEProcessingDialog::showError(const QString &error)
{
    this->m_movie->stop();
    this->ui->prompt->setText(error);
    this->ui->closeButton->setVisible(true);
}

void HPEProcessingDialog::accept()
{
    m_movie->stop();
    ui->closeButton->setVisible(false);
    QDialog::accept();
}

void HPEProcessingDialog::showEvent(QShowEvent *)
{
    //ADD SETTING: timeout
    QTimer::singleShot(600000, this, [this](){
        showError(tr("Processing TIMEOUT or ERROR!!"));
    });
}

void HPEProcessingDialog::resizeEvent(QResizeEvent *)
{
    moveToCenter();
}
