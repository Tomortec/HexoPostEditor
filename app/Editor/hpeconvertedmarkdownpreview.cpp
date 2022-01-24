/**
 * @file hpeconvertedmarkdownpreview.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-22
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpeconvertedmarkdownpreview.h"

#include <QTextBlock>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QRegularExpression>

#include "hpemarkdowneditor.h"
#include "Controller/hpesettings.h"

HPEConvertedMarkdownPreview::HPEConvertedMarkdownPreview(HPEMarkdownEditor *connectedEditor, QWidget *parent)
    : QWidget{parent}
{
    m_preview = new HPEMarkdownEditor(this);
    if(!this->layout())
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        this->setLayout(layout);
    }
    this->layout()->addWidget(m_preview);

    m_preview->setReadOnly(true);
    QFont font;
    font.fromString(HPESettings::config()->value("markdownField/font").toString());
    m_preview->setFont(font);
    connectEditor(connectedEditor);
}

void HPEConvertedMarkdownPreview::connectEditor(HPEMarkdownEditor *connectedEditor)
{
    m_connectedEditor = connectedEditor;
    if(m_connectedEditor)
    {
        m_connectedDocument = m_connectedEditor->document();
        //connect(m_connectedDocument, &QTextDocument::contentsChange, this, &HPEConvertedMarkdownPreview::convertBlock);
        connect(m_connectedDocument, &QTextDocument::contentsChanged, this, &HPEConvertedMarkdownPreview::analyze);
        connect(m_connectedEditor->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int v){
            m_preview->verticalScrollBar()->setValue(v);
        });
    }
}

void HPEConvertedMarkdownPreview::analyze()
{
    if(m_currentFileDir == QDir())
        return;

    m_preview->clear();
    analyzeFrontMatter();
    if(m_connectedDocument)
    {
        QTextBlock block = m_connectedDocument->findBlockByNumber(m_frontMatterBlockRange.getTo()).next();
        while(block.isValid())
        {
            m_preview->appendPlainText(convertBlock(block));
            QTextCursor cursor(m_preview->textCursor());
            cursor.movePosition(QTextCursor::NextBlock);
            m_preview->setTextCursor(cursor);
            block = block.next();
        }
        //move to
        //m_preview->setTextCursor(m_connectedEditor->textCursor());
    }
    m_convertedMarkdown = m_preview->toPlainText();
    emit convertedAll(m_convertedMarkdown);
}

void HPEConvertedMarkdownPreview::filePathChanged(const QString &path)
{
    m_currentFilePath = path;
    m_currentFileDir  = QFileInfo(path).absoluteDir();
    m_sourceDir = m_currentFileDir;
    if(m_sourceDir.dirName() != "source")
    {
        //if its upper dir is 'source', move to it
        if(!m_sourceDir.cdUp() || m_sourceDir.dirName() != "source")
        {
            emit errorOccurred(tr("incorrect path"));
            return;
        }
    }

    analyze();
}

void HPEConvertedMarkdownPreview::analyzeFrontMatter()
{
    if(m_connectedEditor && m_connectedDocument)
    {
        QTextCursor fmStartCursor = m_connectedDocument->find(QRegularExpression("^---$"));
        int fmStart = fmStartCursor.blockNumber();
        int fmEnd = m_connectedDocument->find(QRegularExpression("^---$"), fmStartCursor.movePosition(QTextCursor::NextBlock)).blockNumber();
        m_frontMatterBlockRange.setRange(fmStart, fmEnd);

        convertTitle();
    }
}

void HPEConvertedMarkdownPreview::convertTitle()
{
    //get title of the post
    QRegularExpressionMatch match;
    for (int i = m_frontMatterBlockRange.getFrom() + 1;i < m_frontMatterBlockRange.getTo(); ++i)
    {
        match = QRegularExpression("^[ ]*title: (.+)$").match(m_connectedDocument->findBlockByNumber(i).text());
        if(match.hasMatch())
        {
            m_title = match.capturedTexts().at(1);
            QTextCursor cursor(m_preview->textCursor());
            cursor.movePosition(QTextCursor::Start);

            //remove previous title
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            m_preview->setTextCursor(cursor);

            cursor.movePosition(QTextCursor::Start);
            m_preview->setTextCursor(cursor);
            m_preview->insertPlainText(QString("# %1").arg(m_title));

            //cursor.movePosition(QTextCursor::NextBlock);
            //m_preview->setTextCursor(cursor);
            break;
        }
    }
}

QString HPEConvertedMarkdownPreview::convertBlock(const QTextBlock &block)
{
    if(m_frontMatterBlockRange.contains(block.blockNumber()))
    {
        analyzeFrontMatter();
        return "";
    }

    QString targetText = block.text();
    QRegularExpression imagePattern = QRegularExpression(QString("\\!\\[.*\\]\\((.+)\\)"));
    QRegularExpressionMatch imageMatch =imagePattern.match(targetText);
    if(imageMatch.hasMatch())
    {
        QString assetsFolderName = QFileInfo(m_currentFilePath).baseName();
        QString imagePath = imageMatch.capturedTexts().at(1);
        if(!QRegularExpression("^[a-zA-z]+://[^\\s]+$").match(imagePath).hasMatch())
        {
            QString targetFile;
            if(imagePath.startsWith("/images"))
                targetFile = m_sourceDir.absolutePath() + imagePath;
            else
                targetFile = m_currentFileDir.absolutePath() + QString("/%1/%2").arg(assetsFolderName, imagePath);
            targetText.replace(imageMatch.capturedStart(1), imageMatch.capturedLength(1),
                                    QUrl::fromLocalFile(targetFile).toString());

        }
        //else if it's url, remain the same
        return targetText;
    }
    else
        return block.text();
}
