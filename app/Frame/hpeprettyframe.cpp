/**
 * @file hpeprettyframe.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpeprettyframe.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

HPEPrettyFrame::HPEPrettyFrame(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f)
{
    //this->setStyleSheet("border-radius: 18px; border: 5px inset gray;");
}

void HPEPrettyFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(event->rect(), 15.0, 15.0);
    painter.fillPath(path, QColor(0, 0, 0, 30));
    this->setContentsMargins(3, 3, 3, 3);
}
