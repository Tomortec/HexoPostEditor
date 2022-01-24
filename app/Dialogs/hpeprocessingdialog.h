/**
 * @file hpeprocessingdialog.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEPROCESSINGDIALOG_H
#define HPEPROCESSINGDIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class HPEProcessingDialog;
}

/**
 * @class HPEProcessingDialog
 * @brief A dialog used to inform user of loading
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * 
 * HPEProcessingDialog is a QDialog which will always be modal.
 * It has a caption and a loading GIF animation (app/resources/anim_processing.gif).
 * 
 * HPEProcessingDialog will show TIMEOUT error if it works too long
 * in case of blocking main window unstoppably.
 * 
 * To change the caption, use setPrompt().
 * 
 * To show error, use showError(), which will provide a close button.
 * 
 * @see QDialog
*/
class HPEProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new HPEProcessingDialog object with initial prompt
     * 
     * @param[in] prompt Initial caption
     * @param[in] parent 
    */
    explicit HPEProcessingDialog(const QString& prompt = "", QWidget *parent = nullptr);

    /**
     * @brief Destroy HPEProcessingDialog UI
     * 
    */
    ~HPEProcessingDialog();

    /**
     * @brief Set the caption of ui->prompt
     * 
     * @param[in] prompt 
    */
    void setPrompt(const QString&);

private:
    Ui::HPEProcessingDialog *ui;

    /**
     * @brief This property is used to load and play GIF
     * 
    */
    QMovie* m_movie;

    /**
     * @brief Move HPEProcessingDialog to the center of screen
     * 
    */
    void moveToCenter();

public slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Set the text of ui->prompt with error string
     * and show ui->closeButton for user to close HPEProcessingDialog
     * 
     * @param[in] error
    */
    void showError(const QString&);

    /**
     * @brief Overrides QDialog::accept() 
     * to reset UI
    */
    void accept() override;
/**
 * @}
*/
protected:

    /**
     * @brief Overrides QDialog::showEvent() to set a Timer
     * which will show TIMEOUT error if HPEProcessingDialog works too long,
     * in case of blocking main window unstoppably.
     * 
    */
    void showEvent(QShowEvent*) override;

    /**
     * @brief Overrides QDialog::resizeEvent() to
     * make sure HPEProcessingDialog stays at the center of screen all the time
     * 
    */
    void resizeEvent(QResizeEvent*) override;
};

#endif // HPEPROCESSINGDIALOG_H
