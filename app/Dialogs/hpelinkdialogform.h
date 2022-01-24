/**
 * @file hpelinkdialogform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPELINKDIALOGFORM_H
#define HPELINKDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPELinkDialogForm;
}

/**
 * @class HPELinkDialogForm
 * @brief A widget used to show inserted links and insert links
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * HPELinkDialogForm is usually embedded in HPEDialog.
 * HPELinkDialogForm has a QTableWidget to show inserted links
 * and a widget to insert link.
 * 
 * To access the name and the site of the link,
 * use getLinkName() and getLinkSite()
 * 
 * To reset UI, use resetEdits()
 * 
 * In order to get all inserted links, HPELinkDialogForm overrides showEvent() which
 * will find HPEMarkdownEditor and call its HPEMarkdownEditor::getDocumentLinks()
 * 
 * @see HPEDialog, HPEMarkdownEditor::getDocumentLinks()
*/
class HPELinkDialogForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new HPELinkDialogForm object with parent
     * 
     * @param[in] parent 
    */
    explicit HPELinkDialogForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPELinkDialogForm UI
     * 
    */
    ~HPELinkDialogForm();

private:
    Ui::HPELinkDialogForm *ui;

public:

    /**
     * @brief Get the name of the link from ui->nameEdit
     * 
     * @return The name of the link 
    */
    QString getLinkName() const;

    /**
     * @brief Get the site of the link from ui->siteEdit
     * 
     * @return The site of the link
    */
    QString getLinkSite() const;

    /**
     * @brief Reset all edits
     * 
    */
    void resetEdits();

protected:

    /**
     * @brief Overrides QWidget::showEvent() to 
     * get all inserted links from HPEMarkdownEditor 
     * to provide data for ui->linksList
     * 
    */
    void showEvent(QShowEvent*) override;
};

#endif // HPELINKDIALOGFORM_H
