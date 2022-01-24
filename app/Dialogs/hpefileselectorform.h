/**
 * @file hpefileselectorform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEFILESELECTORFORM_H
#define HPEFILESELECTORFORM_H

#include <QWidget>
#include <QDir>

namespace Ui {
class HPEFileSelectorForm;
}

/**
 * @class HPEFileSelectorForm
 * @brief A widget embedded in HPEStartupDialog, for opening files.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * @par Construction
 * 
 * An HPEFileSelectorForm is embedded in an HPEStartupDialog
 * and its parent is usually an HPEStartupDialog,
 * it will be created when the HPEStartupDialog is constructing.
 *
 * HPEFileSelectorForm needs to get the directory (Hexo 'source' folder) user wants to open
 * so that it can list all markdown files in the directory recursively.
 * Therefore, HPEFileSelectorForm will be available only after its setDir() method
 * is called.
 * 
 * \attention HPEFileSelectorForm will be available only after setDir() method
 * is called.
 * 
 * @par File Selection
 * 
 * After the confirm button is clicked or the list item is double-clicked,
 * HPEFileSelectorForm will emit confirm() signal which will always
 * be captured by HPEStartupDialog to tell which file should open.
 * 
 * @see HPEStartupDialog
*/
class HPEFileSelectorForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct an HPEFileSelectorForm with parent.
     * 
     * @param[in] parent The QObject parent
    */
    explicit HPEFileSelectorForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEFileSelectorForm UI
     * 
    */
    ~HPEFileSelectorForm();

private:
    Ui::HPEFileSelectorForm *ui;

    /**
     * @brief Stores the directory to iterate.
     * This property should be set only by calling setDir().
     * This property is essential to HPEFileSelectorForm.
     * 
     * @see setDir()
    */
    QDir m_targetDir;

    /**
     * @brief     Stores the selected relative file path in ui->fileListWidget
     * so that HPEFileSelectorForm knows which file path to be emitted by confirm()
     * 
    */
    QString m_selectedFileName;

public:

    /**
     * @brief This method is usually called by HPEStartupDialog.
     * Store the given directory and list all Markdown files
     * in the directory in ui->fileListWidget.
     * Note that HPEFileSelectorForm won't be available until this method is called.
     * 
     * @param[in] directory The directory to iterate.
     * 
     * @attention HPEFileSelectorForm won't be available until this method is called.
    */
    void setDir(const QDir&);

    /**
     * @brief Reset UI's status
     * 
    */
    void reset();

signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when the user click ui->backButton.
     * 
     * This signal is usually captured by HPEStartupDialog and will hide this widget.
     * 
    */
    void back();

    /**
     * @brief This signal will emit the selected file's absolute path.
     * @see HPEFileSelectorForm::m_selectedFileName
     * 
     * This signal is emitted when the user click ui->confirmButton
     * or double-click the list item in ui->fileListWidget.
     * 
     * This signal is usually captured by HPEStartupDialog.
     * 
     * @pre setDir() is called
     * 
    */
    void confirm(const QString&);

/**
 * @}
*/
};

#endif // HPEFILESELECTORFORM_H
