/**
 * @file hpetabledialogform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPETABLEDIALOGFORM_H
#define HPETABLEDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPETableDialogForm;
}

/**
 * @class HPETableDialogForm
 * @brief A widget used to insert Markdown table
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * HPETableDialogForm is usually embedded in HPEDialog.
 * It has a QTableWidget to get user's inputs.
 * It also has two QSpinBox to change the number of row and column
 * 
 * To get the data user input, call getTableContent().
 * 
 * To reset UI, call resetTable().
 * 
 * @see QTableWidget
*/
class HPETableDialogForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new HPETableDialogForm with parent
     * 
     * @param[in] parent 
    */
    explicit HPETableDialogForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPETableDialogForm UI
     * 
    */
    ~HPETableDialogForm();

private:
    Ui::HPETableDialogForm *ui;

public:

    /**
     * @brief This static property is used to get the size of
     * the longest data the user input. It can be used in 
     * string format to make Markdown table more pretty.
     * 
    */
    static int maxLengthText;

public:

    /**
     * @brief This method will return a 2d string array
     * whose width and height depend on the column and row number of ui->tableWidget, 
     * even if some of the table's cells are empty.
     * That is, the returned array will not ignore empty table cells.
     * 
     * @attention The returned array will not ignore empty table cells.
     * 
     * @return 2 dimension string array
    */
    QList<QList<QString> > getTableContent() const;

    /**
     * @brief Clear table
     * 
    */
    void resetTable();
};

#endif // HPETABLEDIALOGFORM_H
