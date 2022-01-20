/**
 * @file hpestartupdialog.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPESTARTUPDIALOG_H
#define HPESTARTUPDIALOG_H

#include <QDialog>
#include <QDir>

namespace Ui {
class HPEStartupDialog;
}

class HPEHexoController;
class HPEFileSelectorForm;
class HPEFileCreatorForm;
class QTerminalProcess;

/**
 * @class HPEStartupDialog
 * @brief A dialog used to guide users to create or open files
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * 
 * @par Brief
 * 
 * A HPEStartupDialog is used to guide users to create or open files.
 * 
 * @par UI
 * 
 * A HPEStartupDialog has a fixed size, of 575's width and 375's height,
 * and is always a \l{QDialog::modal}{modal dialog} with a Qt::Dialog flag.
 * 
 * The HPEStartupDialog's UI is designed in hpestartupdialog.ui
 * 
 * A HPEStartupDialog provides a QLineEdit to get the user's
 * Hexo project's directory, after editting, the HPEStartupDialog
 * will call m_dirValidator to check if the directory is valid.
 * 
 * @see HPEHexoController::checkHexoInstallation()
 * 
 * @par Switching between two child forms
 * 
 * A HPEStartupDialog has two child forms, one for creating a new hexo post,
 * one for opening an existing hexo post. These two forms are implemented
 * as HPEFilesCreatorForm and HPEFilesSelectorForm.
 * 
 * The switch between the two forms is implemented by loadFileCreatorForm()
 * and loadFileSelectorForm().
 * 
 * @see QDialog
*/
class HPEStartupDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs a HPEStartupDialog with parent parent.
     * Will set the dialog's fixed size and set ui->dirEdit with last stored Hexo dir.
     * After initialization, will check Hexo environment and the validation of the
     * Hexo project dir once.
     * 
     * @param[in] parent 
    */
    explicit HPEStartupDialog(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEStartupDialog UI
     * 
    */
    ~HPEStartupDialog();

private:
    Ui::HPEStartupDialog *ui;

    /**
     * @brief This property holds the Hexo 'source' directory to work in.
     * 
    */
    QDir m_targetDir;

    /**
     * @brief This property holds the absolute path to be opened.
     * 
     * @attention This property makes sense only after the
     * receiveTargetFilePath(const QString&) or accept() signal
     * is emitted.
     * 
     * @see QDialog::accept()
    */
    QString m_targetFile;

    /**
     * @brief This property is used to check if the dir received is valid.
     * It will show an HPEProcessingDialog and emit hexoEnvironmentChecked() if the directory is valid.
     * In this class, its HPEHexoController::checkHexoInstallation() and HPEHexoController::setDir()
     * are mostly called.
     * 
     * @see HPEHexoController::hexoEnvironmentChecked(), HPEHexoController::checkHexoInstallation(), HPEHexoController::setDir()
    */
    HPEHexoController* m_dirValidator;

    /**
     * @brief The property PEStartupDialog::m_dirValidator relies on this property.
     * 
     * @see QTerminalProcess, HPEHexoController
    */
    QTerminalProcess* m_commandFinder;

    HPEFileSelectorForm* m_fileSelectorForm;
    HPEFileCreatorForm*  m_fileCreatorForm;

public:
    /**
     * @brief Returns the file(usually the file to be opened) path got from the current form.
     * 
     * @return The file(usually the file to be opened) path got from the current form.
    */
    QString getTargetFile() const;

private:

    /**
     * @brief Hide all child forms and show initial central widget
     * 
    */
    void backToDirSelectorWidget();

    /**
     * @brief Hide initial central widget
     * 
    */
    void hideDirSelectorWidget();

private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Open a QFileDialog to get the Hexo project directory
     * and ensure the dir received locates in Hexo's 'source' folder.
     * 
     * If the directory is literally valid, show in ui->dirEdit and pass it
     * to m_dirValidator to check if Hexo is installed in this directory.
     * 
     * @note the result of the m_dirValidator will affect
     * if ui->newFileButton and ui->openFileButton is enabled
     * 
     * @see HPEHexoController::setDir()
    */
    void onOpenDirDialog();

    /**
     * @brief Hide the initial central widget of the HPEStartupDialog, show child form
     * and pass the Hexo 'source' directory to the child form
     * for further process.
     * This slot is executed when the ui->openFileButton is clicked.
     * 
     * @see HPEFileSelectorForm::setDir()
    */
    void loadFileSelectorForm();

    /**
     * @brief Hide the initial central widget of the HPEStartupDialog, show child form
     * and pass the Hexo 'source' directory to the child form
     * for further process.
     * This slot is executed when the ui->newFileButton is clicked.
     * 
     * @see HPEFileCreatorForm::setDir()
    */
    void loadFileCreatorForm();

/**
 * @}
*/
signals:
/**
 * @defgroup signal
 * @{
*/

    /**
     * @brief This signal is emitted when the child form gets the file (to be opened) path,
     * after either HPEFileCreatorForm::fileCreated() or HPEFileSelectorForm::confirm() signal
     * is emitted.
     * 
     * @note This signal is always followed by accept()
     * 
     * @see HPEFileCreatorForm::fileCreated(), HPEFileSelectorForm::confirm(), accept()
    */
    void receiveTargetFilePath(const QString&);

/**
 * @}
*/
};

#endif // HPESTARTUPDIALOG_H
