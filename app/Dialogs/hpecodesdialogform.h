#ifndef HPECODESDIALOGFORM_H
#define HPECODESDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPECodesDialogForm;
}

class HPECodesObject : public QObject
{
    Q_OBJECT
public:
    HPECodesObject(QObject* parent) : QObject(parent) {  }

public slots:
    void getCodesFromWebPage(const QString& codes);
    void changeCodesLanguage(const QString& lang);
    void resetCodesEdit();

signals:
    void sentCodes(const QString&);
    void changeLanguage(const QString&);
    void resetEdit();
};

class HPECodesDialogForm : public QWidget
{
    Q_OBJECT

public:
    explicit HPECodesDialogForm(QWidget *parent = nullptr);
    ~HPECodesDialogForm();

private:
    Ui::HPECodesDialogForm *ui;
    QString m_codes;
    QString m_language;

public:
    QString getCodes() const;
    QString getLanguage() const;
    void resetEdit();

signals:
    void resetCodesEdit();
    void languageChanged(const QString&);

private slots:
    void receiveCodes(const QString&);
    void changeLanguage(const QString&);
};

class HighlightJSLanguages
{
public:
    static const QStringList HighlightJSSupportedLanguages;
};

#endif // HPECODESDIALOGFORM_H
