/**
 * @file hpelinenumberarea.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPELINENUMBERAREA_H
#define HPELINENUMBERAREA_H

#include <QWidget>

class HPEMarkdownEditor;

/**
 * @class HPELineNumberArea
 * @brief A widget set on the left side of an editor, showing line numbers.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup editor
 * 
 * A widget set on the left side of an editor, showing line numbers.
 * This class overrides QWidget's sizeHint() and paintEvent().
 * 
 * Visit {https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html}{Code Editor Example}
 * for more information.
 * 
 * @see QWidget::sizeHint(), QWidget::paintEvent()
*/
class HPELineNumberArea : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief Constructs an HPELineNumberArea with editor.
     * The line number area will be mounted on the editor.
     * 
     * @param[in] editor The editor to be embedded in
     * @param[in] parent NO MEANING. The parent is editor.
    */
    explicit HPELineNumberArea(HPEMarkdownEditor* editor, QWidget *parent = nullptr);

    /**
     * @brief Overrides QWidget::sizeHint()
    */
    QSize sizeHint() const override;

private:
    HPEMarkdownEditor* editor;

protected:

    /**
     * @brief Overrides QWidget::paintEvent().
     * 
    */
    void paintEvent(QPaintEvent*) override;
};

#endif // HPELINENUMBERAREA_H
