#ifndef HPETABLEDIALOGFORM_H
#define HPETABLEDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPETableDialogForm;
}

class HPETableDialogForm : public QWidget
{
    Q_OBJECT

public:
    explicit HPETableDialogForm(QWidget *parent = nullptr);
    ~HPETableDialogForm();

private:
    Ui::HPETableDialogForm *ui;

public:
    static int maxLengthText;

public:
    QList<QList<QString> > getTableContent() const;
    void resetTable();
};

#endif // HPETABLEDIALOGFORM_H
