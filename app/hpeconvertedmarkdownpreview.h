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

class HPEConvertedMarkdownPreview : public QWidget
{
    Q_OBJECT
public:
    explicit HPEConvertedMarkdownPreview(HPEMarkdownEditor* connectedEditor = nullptr, QWidget *parent = nullptr);

private:
    HPEMarkdownEditor* m_preview;

    HPEMarkdownEditor* m_connectedEditor;
    QTextDocument* m_connectedDocument;
    BlockRange m_frontMatterBlockRange;
    QString m_convertedMarkdown;
    QString m_title;
    QString m_currentFilePath;
    QDir m_currentFileDir;
    QDir m_sourceDir;

public:
    void connectEditor(HPEMarkdownEditor*);

public slots:
    void analyze();
    void filePathChanged(const QString&);

private:
    void analyzeFrontMatter();
    void convertTitle();
    QString convertBlock(const QTextBlock&);

signals:
    void convertedAll(const QString&);
    void errorOccurred(const QString&);
};

#endif // HPECONVERTEDMARKDOWNPREVIEW_H
