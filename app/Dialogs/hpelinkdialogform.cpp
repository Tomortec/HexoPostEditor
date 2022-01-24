/**
 * @file hpelinkdialogform.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpelinkdialogform.h"
#include "ui_hpelinkdialogform.h"

#include <QTableWidgetItem>

#include "hpemainwindow.h"
#include "Editor/hpemarkdowneditor.h"

HPELinkDialogForm::HPELinkDialogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPELinkDialogForm)
{
    ui->setupUi(this);

    ui->linksList->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
    ui->linksList->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Site")));
}

HPELinkDialogForm::~HPELinkDialogForm()
{
    delete ui;
}

QString HPELinkDialogForm::getLinkName() const
{
    return ui->nameEdit->text();
}

QString HPELinkDialogForm::getLinkSite() const
{
    return ui->siteEdit->text();
}

void HPELinkDialogForm::resetEdits()
{
    ui->nameEdit->setText("");
    ui->siteEdit->setText("");
}

void HPELinkDialogForm::showEvent(QShowEvent *)
{
    foreach(QWidget* widget, qApp->topLevelWidgets())
    {
        if(HPEMainWindow* mainWindow = qobject_cast<HPEMainWindow*>(widget))
            if(HPEMarkdownEditor* editor = mainWindow->getEditor())
            {
                QList<HPEMarkdownEditor::Link> list = editor->getDocumentLinks();
                ui->linksList->setRowCount(list.length());
                for (int i = 0; i < list.length(); ++i)
                {
                    ui->linksList->setItem(i, 0, new QTableWidgetItem(list[i].name));
                    ui->linksList->setItem(i, 1, new QTableWidgetItem(list[i].site));
                }
            }
    }
}
