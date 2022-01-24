/**
 * @file hpecodesdialogform.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPECODESDIALOGFORM_H
#define HPECODESDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPECodesDialogForm;
}

/**
 * @class HPECodesObject
 * @brief An HPECodesObject is used to get codes from QWebChannel
 * @since 1.0.0
 * 
 * @ingroup controller
 * 
 * HPECodesObject is used to communicate with JS through QWebChannel.
 * It can get texts user inputs in web page and send signal to inform 
 * web page to do something.
 * 
 * @note The corresponding web page is app/resources/codeDialog/codesRendering.html
 * 
 * @see QWebChannel
 * 
*/
class HPECodesObject : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Construct a new HPECodesObject object with parent
     * 
     * @param[in] parent 
    */
    HPECodesObject(QObject* parent) : QObject(parent) {  }

public slots:
/**
 * @defgroup slots
 * @{
*/
    
    /**
     * @brief Emit sendCodes() signal to transfer codes from web page
     * 
     * @param[in] codes 
     * 
     * @see sendCodes()
    */
    void getCodesFromWebPage(const QString& codes);

    /**
     * @brief Emit changeLanguage() signal to inform web page 
     * change codes highlighting
     * 
     * @param[in] lang
     * 
     * @see changeLanguage() 
    */
    void changeCodesLanguage(const QString& lang);

    /**
     * @brief Emit resetEdit() signal to inform web page
     * clear all edits
     * 
     * @see resetEdit()
    */
    void resetCodesEdit();
/**
 * @}
*/
signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief Transfer codes from web page.
     * Usually triggered by getCodesFromWebPage()
     * 
     * @see getCodesFromWebPage()
    */
    void sentCodes(const QString&);

    /**
     * @brief Ask web page to change code highlighting.
     * Usually triggered by changeCodesLanguage()
     * 
     * @see changeCodesLanguage
    */
    void changeLanguage(const QString&);

    /**
     * @brief Ask web page to clear all edits.
     * Usually triggered by resetCodesEdit()
     * 
     * @see resetCodesEdit()
    */
    void resetEdit();
/**
 * @}
*/
};

/**
 * @class HPECodesDialogForm
 * @brief A widget used to highlight and insert codes
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup dialogs
 * @ingroup forms
 * 
 * HPECodesDialogForm is usually embedded in HPEDialog.
 * It has a built-in web page (app/resources/codeDialog/codesRendering.html)
 * to edit and highlight codes.
 * 
 * Users can use the QComboBox to specify the language.
 * This will emit a signal to web page to inform the change of language 
 * through HPECodesObject. The process is done by QWebChannel.
 * 
 * To access the codes and language input into HPECodesDialogForm,
 * use getCodes() and getLanguage()
 * 
 * To reset UI, use resetEdit()
 * 
 * @see HPEDialog, HPECodesObject, QWebChannel
*/
class HPECodesDialogForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new HPECodesDialogForm widget with parent
     * 
     * @param[in] parent 
    */
    explicit HPECodesDialogForm(QWidget *parent = nullptr);

    /**
     * @brief Destroy HPECodesDialogForm UI
     * 
    */
    ~HPECodesDialogForm();

private:
    Ui::HPECodesDialogForm *ui;

    /**
     * @brief The codes input in web page
     * 
    */
    QString m_codes;

    /**
     * @brief The current language in ui->languageSelector
     * 
    */
    QString m_language;

public:

    /**
     * @brief Get the codes input in web page
     * 
     * @return Codes input in web page
    */
    QString getCodes() const;

    /**
     * @brief Get the current language in ui->languageSelector
     * 
     * @return Current language 
    */
    QString getLanguage() const;

    /**
     * @brief Reset codes, language and UI
     * 
     * This method will clear codes, language and 
     * emit resetCodesEdit() signal to ask HPECodesObject
     * to clear edits in web page.
     * 
     * @see resetCodesEdit(), HPECodesObject
    */
    void resetEdit();

signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is triggered when resetEdit() is called.
     * 
     * This signal is triggered when resetEdit() is called
     * and is usually captured by HPECodesObject to ask 
     * web page to clear all edits.
    */
    void resetCodesEdit();

    /**
     * @brief This signal is triggered 
     * when the text in ui->languageSelector changes.
     * 
     * This signal is usually captured by HPECodesObject
     * to inform web page the change of language to be highlighted
    */
    void languageChanged(const QString&);
/**
 * @}
*/
private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief This slot is executed when HPECodesObject gets codes
     * from web page
     * 
     * Sets m_codes
     * 
     * @see m_codes
    */
    void receiveCodes(const QString&);

    /**
     * @brief This slot is executed when ui->languageSelector's text
     * is changed.
     * 
     * Sets m_language
     * 
     * @see m_language
    */
    void changeLanguage(const QString&);
/**
 * @}
*/
};

/**
 * @class HighlightJSLanguages
 * @brief This class lists all languages supported by Highlight.js
 * 
 * @ingroup 3rdParty
 * 
 * HighlightJS v11.0
 * 
 * Visit https://highlightjs.readthedocs.io/en/latest/supported-languages.html
 * to update HighlightJSLanguages.
*/
class HighlightJSLanguages
{
public:
    /**
     * @brief Lists all languages supported by Highlight.js
     * 
    */
    static const QStringList HighlightJSSupportedLanguages;
};

#endif // HPECODESDIALOGFORM_H
