/**
 * @file hpeprettyframe.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEPRETTYFRAME_H
#define HPEPRETTYFRAME_H

#include <QFrame>

/**
 * @class HPEPrettyFrame
 * @brief A frame used to beautify widgets containing frames.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup frame
 * 
 * @par Usage
 * 
 * This class can also be used in QDesigner by creating a QFrame and
 * promote its class to HPEPrettyFrame.
 * 
 * @par How to Beautify
 * 
 * By setting the style sheet or draw something in paintEvent(),
 * all of the frames constructed by HPEPrettyFrame will change its style uniformly.
 * 
 * @see QFrame::setStyleSheet()
*/
class HPEPrettyFrame : public QFrame
{
    Q_OBJECT

public:

    /**
     * @brief Construct a beautified frame.
     * 
     * @param parent 
     * @param f 
     * 
     * @see QFrame::QFrame()
    */
    HPEPrettyFrame(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:

    /**
     * @brief Overrides QFrame::paintEvent() to customize pattern.
     * 
    */
    void paintEvent(QPaintEvent*) override;
};

#endif // HPEPRETTYFRAME_H
