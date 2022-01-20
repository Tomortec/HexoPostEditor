#ifndef HPEMARKDOWNEDITOR_H
#define HPEMARKDOWNEDITOR_H

#include <QPlainTextEdit>

class HPELineNumberArea;
class HPESyntaxHighlighter;

class HPEMarkdownEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    struct Link
    {
        QString name;
        QString site;

        Link(const QString& name, const QString& site)
            : name(name), site(site) {  }
    };

private:
    HPELineNumberArea* m_lineNumberArea;
    HPESyntaxHighlighter* m_highlighter;
    const QMap<QString, QString> AUTO_COMPLETE_CHARS = {
        { "\"", "\"" }, { "'", "'" }, { "{", "}" }, { "[", "]" },
        { "(", ")" }, { "<", ">" }, { "《", "》" }, { "“", "”" },
        { "「", "」" }, { "`", "`" }
    };

public:
    HPEMarkdownEditor(QWidget *parent = nullptr);

public:
    //return the modified line
    void wrapSelectionWithString(const QString& str);
    void wrapSelectionWithString(const QString& leftStr, const QString& rightStr);
    void insertString(const QString& str, bool atNewLine = false);

    //line number area
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent* event);

    QList<Link> getDocumentLinks();

signals:
    void scrollVerticallyBy(double dy);

protected:
    void resizeEvent(QResizeEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int deltaY);
};

#endif // HPEMARKDOWNEDITOR_H
