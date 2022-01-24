/**
 * @file hpemarkdowneditor.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-23
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpemarkdowneditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>

#include "hpelinenumberarea.h"
#include "hpesyntaxhighlighter.h"

HPEMarkdownEditor::HPEMarkdownEditor(QWidget* parent)
        : QPlainTextEdit(parent)
{
    m_lineNumberArea = new HPELineNumberArea(this);
    m_highlighter = new HPESyntaxHighlighter(document());

    connect(this, &HPEMarkdownEditor::blockCountChanged, this, &HPEMarkdownEditor::updateLineNumberAreaWidth);
    connect(this, &HPEMarkdownEditor::updateRequest, this, &HPEMarkdownEditor::updateLineNumberArea);
    connect(this, &HPEMarkdownEditor::cursorPositionChanged, this, &HPEMarkdownEditor::highlightCurrentLine);

    this->setLineWidth(0);
    this->setFrameShape(QFrame::NoFrame);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //init line number
    this->updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void HPEMarkdownEditor::wrapSelectionWithString(const QString& str)
{
    this->wrapSelectionWithString(str, str);
}

void HPEMarkdownEditor::wrapSelectionWithString(const QString& leftStr, const QString& rightStr)
{
    QTextCursor newCursor(this->textCursor());
    if(this->textCursor().hasSelection())
    {
        int start = this->textCursor().selectionStart();
        int end   = this->textCursor().selectionEnd();
        newCursor.clearSelection();
        newCursor.setPosition(start);
        newCursor.insertText(" " + leftStr);
        newCursor.setPosition(end + 1 + leftStr.length());
        newCursor.insertText(rightStr + " ");
        this->setTextCursor(newCursor);
    }
    else
    {
        int start = this->textCursor().position();
        newCursor.insertText(" " + leftStr + rightStr + " ");
        newCursor.setPosition(start + 1 + leftStr.length());
        this->setTextCursor(newCursor);
    }
}

void HPEMarkdownEditor::insertString(const QString& str, bool atNewLine)
{
    QTextCursor newCursor(this->textCursor());
    if(atNewLine)
    {
        bool isAtLineStart = this->textCursor().atBlockStart();
        if(!isAtLineStart)
            newCursor.movePosition(QTextCursor::Down);

        newCursor.movePosition(QTextCursor::StartOfLine);
        newCursor.insertText(str);

        if(!isAtLineStart)
        {
            newCursor.insertBlock();
            newCursor.movePosition(QTextCursor::Up);
            newCursor.movePosition(QTextCursor::EndOfLine);
        }
        this->setTextCursor(newCursor);
    }
    else
    {
        newCursor.insertText(" " + str);
        this->setTextCursor(newCursor);
    }
}

/**
 * @brief HPEMarkdownEditor::lineNumberAreaWidth
 * @return
 *
 * calculates the width of the LineNumberArea widget.
 * We take the number of digits in the last line of the editor and multiply that
 * with the maximum width of a digit
 */
int HPEMarkdownEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + this->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void HPEMarkdownEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.setFont(this->font());
    //painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = this->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(0, 0, 0, 30));
            painter.drawText(0, top, m_lineNumberArea->width(), this->fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

QList<HPEMarkdownEditor::Link> HPEMarkdownEditor::getDocumentLinks()
{
    QList<HPEMarkdownEditor::Link> res;
    QRegularExpressionMatchIterator iterator =
            QRegularExpression("\\[(.+)\\]\\((.+)\\)").globalMatch(this->toPlainText());
    while(iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        res.append(HPEMarkdownEditor::Link(match.captured(1), match.captured(2)));
    }

    return res;
}

void HPEMarkdownEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect contentRect = this->contentsRect();
    m_lineNumberArea->setGeometry(QRect(contentRect.left(), contentRect.top(),
                                        this->lineNumberAreaWidth(), contentRect.height()));
}

void HPEMarkdownEditor::wheelEvent(QWheelEvent *event)
{
    this->QPlainTextEdit::wheelEvent(event);
    emit scrollVerticallyBy(double(event->angleDelta().y()) / 8.0); // about the 8.0, see QWheelEvent::angleDelta()
}

void HPEMarkdownEditor::keyPressEvent(QKeyEvent *event)
{
    this->QPlainTextEdit::keyPressEvent(event);
    if(AUTO_COMPLETE_CHARS.contains(event->text()))
    {
        QTextCursor cursor(this->textCursor());
        this->insertPlainText((AUTO_COMPLETE_CHARS.value(event->text())));
        cursor.movePosition(QTextCursor::Left);
        this->setTextCursor(cursor);
    }
}

void HPEMarkdownEditor::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    this->setViewportMargins(this->lineNumberAreaWidth(), 0, 0, 0);
}

void HPEMarkdownEditor::highlightCurrentLine()
{
    QVector<QTextEdit::ExtraSelection> extraSelections;

    if(!this->isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(0, 0, 0, 20);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    this->setExtraSelections(extraSelections);
}

void HPEMarkdownEditor::updateLineNumberArea(const QRect &rect, int deltaY)
{
    if (deltaY)
        m_lineNumberArea->scroll(0, deltaY);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
