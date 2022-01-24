/**
 * @file hpeheadingdialogform.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpeheadingdialogform.h"
#include "ui_hpeheadingdialogform.h"

HPEHeadingDialogForm::HPEHeadingDialogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPEHeadingDialogForm)
{
    ui->setupUi(this);

    connect(ui->addHorizontalRule, &QCheckBox::clicked, this, [=](bool res){ m_addHorizontalRule = res; });
    connect(ui->buttonHeading1, &QPushButton::clicked, this, [=](){ m_headingType = "# "; });
    connect(ui->buttonHeading2, &QPushButton::clicked, this, [=](){ m_headingType = "## "; });
    connect(ui->buttonHeading3, &QPushButton::clicked, this, [=](){ m_headingType = "### "; });
    connect(ui->buttonHeading4, &QPushButton::clicked, this, [=](){ m_headingType = "#### "; });
    connect(ui->buttonHeading5, &QPushButton::clicked, this, [=](){ m_headingType = "##### "; });
    connect(ui->buttonHeading6, &QPushButton::clicked, this, [=](){ m_headingType = "###### "; });

}

HPEHeadingDialogForm::~HPEHeadingDialogForm()
{
    delete ui;
}

QString HPEHeadingDialogForm::getHeadingText() const
{
    return m_headingType + ui->headingEdit->text();
}

bool HPEHeadingDialogForm::isAddHorizontalRule() const
{
    return m_addHorizontalRule;
}

void HPEHeadingDialogForm::resetEdit()
{
    ui->headingEdit->setText("");
}
