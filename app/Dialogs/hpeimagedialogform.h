/**
 * @file hpeimagedialogform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEIMAGEDIALOGFORM_H
#define HPEIMAGEDIALOGFORM_H

#include <QWidget>
#include <QGroupBox>

namespace Ui {
class HPEImageDialogForm;
}

class HPEMainWindow;

/**
 * @class HPEImageDialogForm
 * @brief A widget used to insert images
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * HPEImageDialogForm is usually embedded in HPEDialog.
 * HPEImageDialogForm provides two ways to insert image.
 * One is inserting images that have already been added to Hexo assets folder,
 * the other is inserting images in other directories.
 * The latter will copy images to Hexo assets folder. 
 * 
 * Visit {https://hexo.io/docs/asset-folders} to learn more about Assets Folder in Hexo.
 * 
 * To get the target image path, call getImageFilePath().
 * 
 * To reset UI, call resetEdit()
 * 
 * @see HPEDialog
*/
class HPEImageDialogForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new HPEImageDialogForm object with parent
     * 
     * @param[in] parent 
    */
    explicit HPEImageDialogForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEImageDialogForm UI
     * 
    */
    ~HPEImageDialogForm();

private:
    Ui::HPEImageDialogForm *ui;

    /**
     * @brief This property stores the original absolute path of an image
     * if user choose to insert from other dirs
     * 
    */
    QString m_imagePath;

    /**
     * @brief The current main window.
     * 
     * This class needs to call HPEMainWindow::getFilePath()
    */
    HPEMainWindow* m_mainWindow;

public:

    /**
     * @brief If user choose to insert from Hexo assets folder, this method will 
     * standardize the image path. If user choose to insert from other folders, this method
     * will copy the file and return a standard Hexo image path.
     * 
     * @note If something goes wrong, this method will return unprocessed path
     * 
     * @return Image path in the style of Hexo (assets folder)
    */
    QString getImageFilePath() const;

    /**
     * @brief Reset UI
     * 
    */
    void resetEdit();

protected:

    /**
     * @brief Overrides QWidget::showEvent() to 
     * iterate all supported images in assets folder.
    */
    void showEvent(QShowEvent*) override;

private:

    /**
     * @brief Change the pattern of QGroupBox based on whether it's checked
     * 
     * @param[in] target
    */
    void beautifyGroupBox(QGroupBox*);

private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief This slot is executed when ui->openFileButton is clicked.
     * If user choose to insert image from other directories,
     * open a file dialog to get the image path and change m_imagePath
     * 
    */
    void onOpenFileDialog();
/**
 * @}
*/
signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is triggered when user double-click ui->imageList's item
     * 
    */
    void imageSelected();
/**
 * @}
*/
};

#endif // HPEIMAGEDIALOGFORM_H
