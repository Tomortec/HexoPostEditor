/**
 * @file hpesyntaxhighlighter.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPESYNTAXHIGHLIGHTER_H
#define HPESYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QTextCharFormat>

class HPEHexoPostAnalyzer;

/**
 * @class HPESyntaxHighlighter
 * @brief A markdown syntax highlighter
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup editor
 * 
 * Render the editor's (document) text by using
 * QRegularExpression and QTextCharFormat.
 * 
 * @par Add Single Line Matching and Rendering Rules
 * 
 * For single line regular expression match, the matching and rendering rules
 * are stored in HPESyntaxHighlighter::highlightingRules.
 * 
 * Therefore, to add rendering rules that match a single line at a time, just
 * append the RE matching rules and char rendering to the HPESyntaxHighlighter::highlightingRules.
 * 
 * @attention The adding process should be done in HPESyntaxHighlighter's constructor.
 * 
 * @par Add multi-Line Matching and Rendering Rules
 * 
 * Should match in highlightBlock()
 * 
 * For more information, visit {https://doc.qt.io/qt-6/qtwidgets-richtext-syntaxhighlighter-example.html}{Syntax Highlighter Example}
*/
class HPESyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    /**
     * @brief Constructs an HPESyntaxHighlighter with parent to be highlighted.
     * All of the single-line-matching rules and rendering rules are defined in
     * this constructor.
     * 
     * @param[in] parent 
    */
    explicit HPESyntaxHighlighter(QTextDocument *parent);

protected:

    /**
     * @brief Overrides QSyntaxHighlighter::highlightBlock()
     * 
     * @param[in] text 
    */
    void highlightBlock(const QString &text) override;

private:

    struct HighlightRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    /**
     * @brief This property stores all single-line-matching and rendering rules
     * 
    */
    QVector<HighlightRule> highlightingRules;

    QTextCharFormat headingFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat underlineFormat;
    QTextCharFormat strikeFormat;
    QTextCharFormat quoteFormat;
    QTextCharFormat hRuleFormat;
    QTextCharFormat insertionFormat;

    QTextCharFormat frontMatterFormat;
    QTextCharFormat tagPluginFormat;
};

#endif // HPESYNTAXHIGHLIGHTER_H
