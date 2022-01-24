/**
 * @file hpeheadingdialogform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEHEADINGDIALOGFORM_H
#define HPEHEADINGDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPEHeadingDialogForm;
}

/**
 * @class HPEHeadingDialogForm
 * @brief A widget used to add Markdown heading
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * HPEHeadingDialogForm is usually embedded in HPEDialog.
 * This widget provides 6 QButton which is auto-exclusive.
 * When the user selects one of them, this widget will 
 * record the current heading type and wait for user's confirm on HPEDialog.
 * 
 * To access heading contents the user inputs,
 * use getHeadingText().
 * 
 * @attention getHeadingText() will return texts with Markdown style. 
 * 
 * To know if the user wants an horizontal rule, call isAddHorizontalRule()
 * 
 * To reset UI, use resetEdit()
 * 
 * @see HPEDialog
*/
class HPEHeadingDialogForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new HPEHeadingDialogForm object with parent
     * 
     * @param[in] parent 
    */
    explicit HPEHeadingDialogForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEHeadingDialogForm UI
     * 
    */
    ~HPEHeadingDialogForm();

private:
    Ui::HPEHeadingDialogForm *ui;

    /**
     * @brief This property holds whether the user wants to add an hRule.
     * By default, this property is false.
     * 
     * This property is connected with ui->addHorizontalRule QCheckBox
    */
    bool m_addHorizontalRule = false;

    /**
     * @brief Holds the heading prefix.
     * By default, this property is "# "
     * in case of null selection
    */
    QString m_headingType = "# ";

public:
    /**
     * @brief Get heading contents the user inputs with Markdown style
     * 
     * @return Heading contents the user inputs with Markdown style 
     * 
     * @attention Will return texts with Markdown style. 
    */
    QString getHeadingText() const;

    /**
     * @brief If the user wants an horizontal rule
     * 
     * @return true if ui->addHorizontalRule is checked
    */
    bool isAddHorizontalRule() const;

    /**
     * @brief Reset UI, but does not unselect ui->buttonHeadings
     * 
    */
    void resetEdit();
};

#endif // HPEHEADINGDIALOGFORM_H
