#ifndef HPELINKDIALOGFORM_H
#define HPELINKDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPELinkDialogForm;
}

class HPELinkDialogForm : public QWidget
{
    Q_OBJECT

public:
    explicit HPELinkDialogForm(QWidget *parent = nullptr);
    ~HPELinkDialogForm();

private:
    Ui::HPELinkDialogForm *ui;

public:
    QString getLinkName() const;
    QString getLinkSite() const;

    void resetEdits();

protected:
    void showEvent(QShowEvent*) override;
};

#endif // HPELINKDIALOGFORM_H
