/**
 * @file hpefilecreatorform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEFILECREATORFORM_H
#define HPEFILECREATORFORM_H

#include <QWidget>
#include <QMap>
#include <QDir>

namespace Ui {
class HPEFileCreatorForm;
}

class HPEHexoController;

/**
 * @class HPEFileCreatorForm
 * @brief A widget embedded in HPEStartupDialog, for creating new Hexo post.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * @par Construction
 * 
 * An HPEFileCreatorForm is embedded in an HPEStartupDialog
 * and its parent is usually an HPEStartupDialog,
 * it will be created when the HPEStartupDialog is constructing.
 * 
 * HPEFileCreatorForm relies on an HPEHexoController to create new Hexo posts.
 * 
 * @attention HPEFileCreatorForm will be available only after setDir() method
 * is called.
 * 
 * @par Process
 * 
 * After getting the directory to create new post from its parent by calling setDir(),
 * HPEFileCreatorForm will be available.
 * 
 * Once the user click confirmButton, HPEFileCreatorForm will call HPEHexoController::createPost()
 * to create new post and listen to HPEHexoController::processFinished() signal
 * which will be emitted when the command works successfully.
 * 
 * Once HPEHexoController::processFinished() signal is captured, HPEFileCreatorForm
 * will check if the file truly exists. If true, emit fileCreated() signal
 * which will be always captured by its parent HPEStartupDialog.
 * 
 * @see HPEStartupDialog, HPEHexoController::createPost(), HPEHexoController::processFinished()
*/
class HPEFileCreatorForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct HPEFileCreatorForm with an HPEHexoController and parent.
     * 
     * @param[in] hexoController The Hexo controller HPEFileCreatorForm use to create post
     * @param[in] parent The QObject parent
    */
    explicit HPEFileCreatorForm(HPEHexoController* hexoController, QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEFileCreatorForm UI
     * 
    */
    ~HPEFileCreatorForm();

private:

    /**
     * @brief Lists all Hexo built-in layout types.
     * 
     * @see LAYOUT_ARGUMENTS
    */
    const QStringList LAYOUT_TYPES
    {
        "DEFAULT", "POST", "PAGE", "DRAFT"
    };

    /**
     * @brief Map HPEFileCreatorForm::LAYOUT_TYPES with its real argument.
     * 
     * @see LAYOUT_TYPES
    */
    const QMap<QString, QString> LAYOUT_ARGUMENTS = {
        {"DEFAULT", ""}, {"POST", "post"},
        {"PAGE", "page"}, {"DRAFT", "draft"}
    };

private:
    Ui::HPEFileCreatorForm *ui;

    /**
     * @brief This property is essential to createNewFile() and fileCreated() signal.
     * 
     * @see createNewFile(), fileCreated()
    */
    HPEHexoController* m_hexoController;

    /**
     * @brief Store the new file's absolute path
     * 
    */
    QString m_newFile;

    /**
     * @brief Stores the directory to confirm if new file has created.
     * This property should be set only by calling setDir().
     * This property is essential to HPEFileCreatorForm.
     * 
     * @attention This property is essential to HPEFileCreatorForm.
     * 
     * @see setDir(), fileCreated()
    */
    QDir m_targetDir;

public:

    /**
     * @brief Set the m_targetDir and update UI.
     * This method should be called after
     * its HPEFileCreatorForm::m_hexoController has called HPEHexoController::setDir,
     * because this method won't call it for security.
     * 
     * Note that HPEFileSelectorForm won't be available until this method is called.
     * 
     * @param[in] directory Hexo 'source' directory
     * 
     * @attention HPEFileSelectorForm won't be available until this method is called.
     * 
     * @see HPEHexoController::setDir()
    */
    void setDir(const QDir&);

    /**
     * @brief Reset UI's status
     * 
    */
    void reset();

private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Call HPEFileCreatorForm::m_hexoController to create new post
     * with the name given in ui->titleEdit and the layout name given in ui->layoutSelector.
     * This slot is executed when ui->confirmButton is called.
     * 
     * @see HPEHexoController::createPost()
     * 
     * @pre setDir() is called
     * 
    */
    void createNewFile();

/**
 * @} 
*/
signals:
/**
 * @defgroup signal
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
     * @brief This signal will emit the created post's absolute filePath.
     * This signal is emitted when the HPEFileCreatorForm::m_hexoController
     * processes successfully and the new file's existence got confirmed.
     * 
     * This signal is usually captured by HPEStartupDialog.
     * 
    */
    void fileCreated(const QString&);

/**
 * @}
*/
};

#endif // HPEFILECREATORFORM_H
