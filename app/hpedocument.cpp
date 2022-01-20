/**
 * @file hpedocument.cpp
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpedocument.h"

HPEDocument::HPEDocument(QObject *parent)
    : QObject{parent} { }

void HPEDocument::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;
    //qDebug() << m_text;
    emit textChanged(m_text);
}

