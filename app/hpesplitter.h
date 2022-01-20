/**
 * @file hpesplitter.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPESPLITTER_H
#define HPESPLITTER_H

#include <QSplitter>

/**
 * @class HPESplitter
 * @brief A splitter with customized handle.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup frame
 * 
 * HPESplitter derives from QSplitter and has a customized handle.
 * The handle is constructed by HPESplitterHandle.
 * 
 * @see HPESplitterHandle
*/
class HPESplitter : public QSplitter
{
    Q_OBJECT
public:

    /**
     * @brief Constructs a HPESplitter with parent and orientation.
     * By default, the orientation is Qt::Horizontal.
     * 
     * @param[in] parent 
     * @param[in] orientation 
    */
    HPESplitter(QWidget* parent = nullptr, Qt::Orientation orientation = Qt::Orientation::Horizontal);

protected:

    /**
     * @brief Overrides QSplitter::createHandle().
     * Create an HPESplitterHandle.
     * 
     * @return QSplitterHandle* 
    */
    QSplitterHandle* createHandle() override;
};

/**
 * @class HPESplitterHandle
 * @brief A customized splitter handle
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup frame
 * 
 * HPESplitterHandle is provided for HPESplitter.
 * To beautify the handle, implement the paintEvent()
 * 
 * @see HPESplitter
*/
class HPESplitterHandle : public QSplitterHandle
{
public:
    /**
     * @brief Constructs a customized HPESplitterHandle.
     * 
     * @param[in] orientation 
     * @param[in] parent 
    */
    HPESplitterHandle(Qt::Orientation orientation, QSplitter* parent);

protected:

    /**
     * @brief Overrides QSplitterHandle::paintEvent().
     * Draw things here.
     * 
    */
    void paintEvent(QPaintEvent*) override;
};

#endif // HPESPLITTER_H
