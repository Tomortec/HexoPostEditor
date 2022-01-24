/**
 * @file hpedialog.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-22
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEDIALOG_H
#define HPEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>

namespace Ui {
class HPEDialog;
}

/**
 * @class HPEDialog
 * @brief A dialog used to load dialog forms to insert Markdown texts
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * 
 * To be honest, this class has structure mistakes.
 * It might be better to write a more generic class.
 * 
 * Generally speaking, HPEDialog is not the base class of all HPEDialog.
 * HPEDialog is used to load dialog forms to insert Markdown texts.
 * 
 * Mostly, HPEDialog is used to load 
 * HPEHeadingDialogForm, HPELinkDialogForm, HPEImageDialogForm, HPECodesDialogForm or HPETableDialogForm.
 * 
 * To construct an HPEDialog with one of the dialog forms mentioned above,
 * DialogType is needed.
 * 
 * HPEDialog will handle different data according to its m_type.
 * It will listen to the form's signal to get data.
 * 
 * Once the DialogType is defined, changing the m_type at runtime may 
 * result in unexpected conditions.
 * 
 * Once user click ui->confirmButton, HPEDialog will handle data
 * and emit dialogConfirm() signal to tranfer processed result.
 * 
 * HPEDialog will emit dialogCancel() signal if user click ui->cancelButton
*/
class HPEDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Defines 5 dialog forms
     * 
    */
    enum DialogType
    {
        HEADING,    //< HPEHeadingDialogForm
        LINK,       //< HPDLinkDialogForm
        IMAGE,      //< HPEImageDialogForm
        CODE,       //< HPECodesDialogForm
        TABLE       //< HPETableDialogForm
    };

public:

    /**
     * @brief Construct a new HPEDialog object with DialogType 
     * 
     * @param[in] type 
     * @param[in] parent 
    */
    explicit HPEDialog(DialogType type, QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEDialog UI
     * 
    */
    ~HPEDialog();

private:
    Ui::HPEDialog *ui;

    /**
     * @brief The type of form. 
     * Once it's assigned, should not be changed
     * 
    */
    DialogType m_type;

    /**
     * @brief Used to load dialog form
     * 
    */
    QWidget* m_centralWidget;
    QHBoxLayout* m_centralWidgetLayout;

    /**
     * @brief Stores processed Markdown text
     * 
    */
    QString m_result;

private:

    /**
     * @brief Executed when DialogType is HEADING.
     * Handle heading data got from HPEHeadingDialogForm
     * 
    */
    void handleHeadingData();

    /**
     * @brief Executed when DialogType is LINK.
     * Handle and validate link data got from HPELinkDialogForm
     * 
     * @note Using QUrl::isValid will check URL literally only
    */
    void handleLinkData();

    /**
     * @brief Executed when DialogType is IMAGE.
     * Handle image data got from HPEImageDialogForm
     * 
    */
    void handleImageData();

    /**
     * @brief Executed when DialogType is CODE.
     * Handle codes data got from HPECodesDialogForm
     * 
    */
    void handleCodesData();

    /**
     * @brief Executed when DialogType is TABLE.
     * Handle table data got from HPETabelDialogForm
     * 
    */
    void handleTableData();

signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted by onConfirm().
     * This signal will transfer processed Markdown text
     * 
     * @see onConfirm()
    */
    void dialogConfirm(const QString& result);

    /**
     * @brief This signal is triggered by onCancel().
     * 
     * @see QDialog::onCancel()
    */
    void dialogCancel();
/**
 * @}
*/
private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Executed when user click ui->confirmButton.
     * Will handle data got from m_centralWidget and emit dialogConfirm()
     * signal. After that, call QDialog::accept().
     * 
     * @see dialogConfirm(), QDialog::accept()
    */
    void onConfirm();

    /**
     * @brief Executed when user click ui->cancelButton.
     * Will emit dialogCancel() signal and call QDialog::reject().
     * 
     * @see QDialog::reject()
    */
    void onCancel();
/**
 * @}
*/
};

#endif // HPEDIALOG_H
