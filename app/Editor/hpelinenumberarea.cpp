/**
 * @file hpelinenumberarea.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpelinenumberarea.h"

#include "hpemarkdowneditor.h"

HPELineNumberArea::HPELineNumberArea(HPEMarkdownEditor* editor, QWidget* /*parent*/)
    : QWidget(editor), editor(editor) {  }

QSize HPELineNumberArea::sizeHint() const
{
    return QSize(editor->lineNumberAreaWidth(), 0);
}

void HPELineNumberArea::paintEvent(QPaintEvent *event)
{
    editor->lineNumberAreaPaintEvent(event);
}
