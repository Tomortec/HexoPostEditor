/**
 * @file hpedocument.h
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEDOCUMENT_H
#define HPEDOCUMENT_H

#include <QObject>

/**
 * @class HPEDocument
 * @brief An HPEDocument is used to expose document texts for QWebChannel
 * @since 1.0.0
 * 
 * @ingroup controller
 * 
 * An HPEDocument object is used to expose document texts for QWebChannel.
 * Visit {https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-markdowneditor-example.html}{Qt WebEngine Markdown Editor Example}
 * for more information.
*/
class HPEDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged FINAL)

public:

    /**
     * @brief Construct an HPEDocument object with parent
     * 
     * @param parent 
    */
    explicit HPEDocument(QObject *parent = nullptr);

    /**
     * @brief After setting the text, will emit textChanged() signal.
     * As the HPEDocument object always serves as a QObject for QWebChannel,
     * the signal will be captured by QWebChannel, and sent to web page for further process.
     * 
     * Visit {https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-markdowneditor-example.html}{Qt WebEngine Markdown Editor Example}
     * for more information.
     * 
     * @see QWebChannel
     * 
     * @param text The text to transfer to JS
    */
    void setText(const QString &text);

signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when setText() method is called.
     * 
     * Usually, this signal will be captured by QWebChannel, and sent to web page for further process.
     * Visit {https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-markdowneditor-example.html}{Qt WebEngine Markdown Editor Example}
     * for more information.
     * 
     * @see QWebChannel
    */
    void textChanged(const QString &text);
/**
 * @}
*/
private:

    /**
     * @brief This property is exposed as 'text' by using Q_PROPERTY.
     * This property will be set by setText().
     * 
     * Visit {https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-markdowneditor-example.html}{Qt WebEngine Markdown Editor Example}
     * for more information.
     * 
    */
    QString m_text;
};

#endif // HPEDOCUMENT_H
