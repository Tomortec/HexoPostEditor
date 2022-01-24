/**
 * @file hpesplitter.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpesplitter.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

HPESplitter::HPESplitter(QWidget* parent, Qt::Orientation orientation)
        : QSplitter(orientation, parent)
{
    setHandleWidth(24);
}

QSplitterHandle* HPESplitter::createHandle()
{
    return new HPESplitterHandle(orientation(), this);
}

HPESplitterHandle::HPESplitterHandle(Qt::Orientation orientation, QSplitter* parent)
        : QSplitterHandle(orientation, parent) { }

void HPESplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    //QPainterPath path;
    QRect rect(event->rect());
    //rect.setWidth(rect.width() - 4*2);
    rect.setLeft(rect.left() + 8);
    rect.setRight(rect.right() - 8);
    /*path.addRoundedRect(rect, 6.0, 6.0);
    painter.fillPath(path, QColor(0, 0, 0, 30));*/

    painter.setPen(QColor(0, 0, 0, 60));
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(rect.center().x() - 11.0, rect.center().y() - 11.0, 24.0, 24.0, Qt::AlignCenter, "❮❯");
}
