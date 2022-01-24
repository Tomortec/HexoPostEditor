/**
 * @file hpesyntaxhighlighter.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpesyntaxhighlighter.h"

HPESyntaxHighlighter::HPESyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    headingFormat.setFontWeight(QFont::Black);

    boldFormat.setFontWeight(QFont::Black);

    italicFormat.setFontItalic(true);

    underlineFormat.setFontUnderline(true);

    strikeFormat.setFontStrikeOut(true);

    quoteFormat.setForeground(QBrush(QColor(0, 0, 0, 60)));

    hRuleFormat.setFontWeight(QFont::Black);

    insertionFormat.setFontItalic(true);
    insertionFormat.setFontUnderline(true);
    insertionFormat.setForeground(QBrush(QColor(0, 0, 0, 60)));

    frontMatterFormat.setFontItalic(true);
    frontMatterFormat.setFontWeight(QFont::Thin);
    frontMatterFormat.setForeground(QBrush(QColor(208, 208, 208)));

    tagPluginFormat.setFontUnderline(true);
    tagPluginFormat.setForeground(QBrush(QColor(208, 208, 208)));

    HighlightRule rule;
    rule.pattern = QRegularExpression("^#+ (.*)$");
    rule.format = headingFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\*([^ ]+)\\*");
    rule.format = italicFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\*\\*([^ ]+)\\*\\*");
    rule.format = boldFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("<u>(.*)</u>");
    rule.format = underlineFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("~~(.*)~~");
    rule.format = strikeFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("^>(.*)$");
    rule.format = quoteFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("^- - -$");
    rule.format = hRuleFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\{% (.+) %\\}");
    rule.format = tagPluginFormat;
    highlightingRules.append(rule);

    const QString insertionRE[] = {
        "\\[(.+)\\]\\((.+)\\)", "<http(.+)>",   //link
        "!\\[(.*)\\]\\((.+)\\)"                //image
    };
    for(const QString &pattern : insertionRE)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format  = insertionFormat;
        highlightingRules.append(rule);
    }
}

void HPESyntaxHighlighter::highlightBlock(const QString &text)
{   
    for (const HighlightRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    //match frontMatter
    // Front-matter is a block of YAML at the beginning of the file

    // OTHER CONTENTS               BlockState: -1
    // ---                                      0
    // FrontMatter contents                     0
    // ···                                      0
    // ---                                      1
    // OTHER CONTENTS               BlockState: -1
    QRegularExpression frontMatterPattern("^---$");
    int index = text.indexOf(frontMatterPattern);
    if(index != -1 && previousBlockState() == -1)    //start
    {
        setFormat(0, 3, frontMatterFormat);
        setCurrentBlockState(0);
    }
    else if(index != -1 && previousBlockState() == 0) //end
    {
        setFormat(0, 3, frontMatterFormat);
        setCurrentBlockState(1);
    }
    else if(index == -1 && previousBlockState() == 0)   //content
    {
        setFormat(0, text.length(), frontMatterFormat);
        setCurrentBlockState(0);
    }
    else
        setCurrentBlockState(-1);
}
