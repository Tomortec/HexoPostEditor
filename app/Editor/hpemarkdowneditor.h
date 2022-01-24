/**
 * @file hpemarkdowneditor.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-23
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEMARKDOWNEDITOR_H
#define HPEMARKDOWNEDITOR_H

#include <QPlainTextEdit>

class HPELineNumberArea;
class HPESyntaxHighlighter;

/**
 * @class HPEMarkdownEditor
 * @brief A markdown editor
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup editor
 * 
 * 
*/
class HPEMarkdownEditor : public QPlainTextEdit
{
    Q_OBJECT
public:

    /**
     * @brief Contains the name and site of a link.
     * 
     * @see getDocumentLinks()
    */
    struct Link
    {
        QString name;
        QString site;

        Link(const QString& name, const QString& site)
            : name(name), site(site) {  }
    };

private:

    /**
     * @brief HPELineNumberArea
     * 
    */
    HPELineNumberArea* m_lineNumberArea;

    /**
     * @brief HPESyntaxHighlighter
     * 
    */
    HPESyntaxHighlighter* m_highlighter;

    /**
     * @brief A constant QMap stores the chars to be completed by editor
     * auto-ly.
     * 
     * @see keyPressEvent()
    */
    const QMap<QString, QString> AUTO_COMPLETE_CHARS = {
        { "\"", "\"" }, { "'", "'" }, { "{", "}" }, { "[", "]" },
        { "(", ")" }, { "<", ">" }, { "《", "》" }, { "“", "”" },
        { "「", "」" }, { "`", "`" }
    };

public:

    /**
     * @brief Construct a new HPEMarkdownEditor object with parent
     * 
     * @param[in] parent 
    */
    HPEMarkdownEditor(QWidget *parent = nullptr);

public:
    
    /**
     * @brief Wrap the selection in editor with str.
     * 
     * @param[in] str 
    */
    void wrapSelectionWithString(const QString& str);

    /**
     * @brief Insert leftStr before current selection and 
     * insert rightStr after selection.
     * 
     * @param[in] leftStr 
     * @param[in] rightStr 
    */
    void wrapSelectionWithString(const QString& leftStr, const QString& rightStr);

    /**
     * @brief This method will insert str in the current text cursor position
     * or at a new line under text cursor, which enables inserting Markdown heading, quote or list more conveniently.
     * 
     * @param[in] str The string to be added
     * @param[in] atNewLine If the str should be added in a new line
    */
    void insertString(const QString& str, bool atNewLine = false);

    /**
     * @brief Calculates the width of the HPELineNumberArea widget. 
     * We take the number of digits in the last line of the editor and multiply that with the maximum width of a digit.
     * 
     * @return The width of the HPELineNumberArea widget.  
     * 
     * @note Visit https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html for more info
    */
    int lineNumberAreaWidth();

    /**
     * @brief Called from HPELineNumberArea whenever it receives a paint event.
     * 
     * @param[in] event Got from HPELineNumberArea
     * 
     * @note Visit https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html for more info
    */
    void lineNumberAreaPaintEvent(QPaintEvent* event);

    /**
     * @brief Iterate the document and get all Links
     * 
     * @return A list of Link written in editor
    */
    QList<Link> getDocumentLinks();

protected:

    /**
     * @brief When the size of the editor changes, the line number area need to resize .
     * 
    */
    void resizeEvent(QResizeEvent*) override;

    /**
     * @brief Emit scrollVerticallyBy() signal
     * 
    */
    void wheelEvent(QWheelEvent*) override;

    /**
     * @brief Detect and complete AUTO_COMPLETE_CHARS
     * 
    */
    void keyPressEvent(QKeyEvent*) override;

private slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief Executed when the number of lines in the editor changes
     * 
     * @param[in] newBlockCount 
     * 
     * @note Visit https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html for more info
    */
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * @brief Whenever, the cursor's position changes, highlight the current line
     * 
     * @note Visit https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html for more info
    */
    void highlightCurrentLine();

    /**
     * @brief This slot is invoked when the editors viewport has been scrolled.
     * 
     * @param[in] rect: the part of the editing area that is do be updated (redrawn)
     * @param[in] deltaY: holds the number of pixels the view has been scrolled vertically
     *
     * @note Visit https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html for more info
    */
    void updateLineNumberArea(const QRect &rect, int deltaY);
/**
 * @}
*/
signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when user scroll HPEMarkdownEditor.
     * 
    */
    void scrollVerticallyBy(double dy);
/**
 * @}
*/
};

#endif // HPEMARKDOWNEDITOR_H
