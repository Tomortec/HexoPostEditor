/**
 * @file hpeconvertedmarkdownpreview.h
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-22
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPECONVERTEDMARKDOWNPREVIEW_H
#define HPECONVERTEDMARKDOWNPREVIEW_H

#include <QWidget>
#include <QDir>
#include <QTextDocument>
#include <QPageRanges>

class HPEMarkdownEditor;

struct BlockRange
{
private:
    int from = 0;
    int to = 0;
public:
    void setRange(int from, int to)
    {
        if(from > to) return;
        this->from = from;
        this->to = to;
    }

    bool contains(int number) { return number >= from && number <= to; }
    int getFrom() { return from; }
    int getTo()   { return to; }
    int length()  { return to - from; }
};


/**
 * @class HPEConvertedMarkdownPreview
 * @brief A widget used to convert Hexo document and show processed texts.
 * @since 1.0.0
 * 
 * @ingroup widgets
 * @ingroup editor
 * 
 * HPEConvertedMarkdownPreview is a widget which can 
 * convert Hexo document (got from HPEMarkdownEditor) to standard Markdown text
 * and show the converted Markdown texts in m_preview(HPEMarkdownEditor).
 * 
 * @attention Providing an HPEMarkdownEditor (either provide in constructor or connectEditor())
 * is essential to other methods.
 * 
 * After converting document to standard Markdown text, 
 * HPEMainWindow can render it correctly through web page (app/resources/index.html).
 * 
 * Afer converting, HPEConvertedMarkdownPreview will emit convertedAll() signal to
 * tranfer converted Markdown texts.
 * 
 * @par Converting rules
 * 
 * 1. analyze Hexo's Front-Matter and extract some useful information, such as
 * title, date, classification and tags.
 * 
 * @note Visit https://hexo.io/docs/front-matter for more info
 * 
 * @todo in this version, only title will be analyzed.
 * 
 * 2. analyze Hexo's assets folder links and convert it
 * to local path.
 * 
 * @note Visit https://hexo.io/docs/asset-folders for more info
 * 
 * 3. analyze Hexo's tag plugin.
 * 
 * @todo in this version, tag plugins are not processed.
 * 
 * @todo Once the text in m_connectedEditor is changed, method analyze()
 * will convert the whole document and return the whole processed document,
 * which might cause performance problem. A better way is to analyze and update one block by one block.
 * 
 * @note Visit https://hexo.io/docs/tag-plugins for more info
*/
class HPEConvertedMarkdownPreview : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief Construct a new HPEConvertedMarkdownPreview with connectedEditor and parent.
     * The connectedEditor can be appointed by connectEditor()
     * 
     * @param[in] connectedEditor The editor to get document from (Can be set by connectEditor())
     * @param[in] parent 
    */
    explicit HPEConvertedMarkdownPreview(HPEMarkdownEditor* connectedEditor = nullptr, QWidget *parent = nullptr);

private:

    /**
     * @brief Used to show converted texts
     * 
    */
    HPEMarkdownEditor* m_preview;

    /**
     * @brief Used to get document
     * 
    */
    HPEMarkdownEditor* m_connectedEditor;

    /**
     * @brief QTextDocument got from m_connectedEditor
     * 
    */
    QTextDocument* m_connectedDocument;

    /**
     * @brief Holds the block number of Front-Matter.
     * 
    */
    BlockRange m_frontMatterBlockRange;

    /**
     * @brief Stores converted Markdown texts
     * 
    */
    QString m_convertedMarkdown;

    /**
     * @brief Stores extracted post title
     * 
    */
    QString m_title;

    /**
     * @brief Stores the file path of current document
     * 
    */
    QString m_currentFilePath;
    QDir m_currentFileDir;

    /**
     * @brief Stores the 'source' folder of current Hexo project.
     * This property is used to link local paths of images stored in assets folder.
     * 
    */
    QDir m_sourceDir;

public:

    /**
     * @brief Set the editor to read document from
     * and bind the target editor's scroll bar with m_preview's scroll bar
     * 
     * @attention This method is essential to other following method
    */
    void connectEditor(HPEMarkdownEditor*);

private:

    /**
     * @brief Find Front-Matter's block number and call convertTitle()
     * 
     * @pre connectEditor() is called
    */
    void analyzeFrontMatter();

    /**
     * @brief Add a heading with title got from Front-Matter 
     * at the start of m_preview
     * 
    */
    void convertTitle();

    /**
     * @brief Handle and convert text in one block.
     * This method will match and convert assets folder links. (and tag plugins)
     * 
     * @return Converted Markdown text in a block
     * 
     * @param[in] targetBlock The block to be processed 
    */
    QString convertBlock(const QTextBlock&);

public slots:
/**
 * @defgroup slots
 * @{
*/

    /**
     * @brief This method will call analyzeFrontMatter() 
     * and convert all blocks in m_connectedDocument by convertBlock().
     * After process, it will emit convertedAll() signal
     * 
     * @see analyzeFrontMatter(), convertBlock()
     * 
    */
    void analyze();

    /**
     * @brief Executed when the file currently open in m_connectedEditor
     * got changed. This will set properties connected with file and call analyze() method to re-render.
     * 
    */
    void filePathChanged(const QString&);
/**
 * @}
*/
signals:
/**
 * @defgroup signals
 * @{
*/

    /**
     * @brief This signal is emitted when analyze()
     * is done and transfer the whole converted Markdown document
     * 
    */
    void convertedAll(const QString&);

    /**
     * @brief Emitted when error occurs and transfer error description
     * 
    */
    void errorOccurred(const QString&);
/**
 * @}
*/
};

#endif // HPECONVERTEDMARKDOWNPREVIEW_H
