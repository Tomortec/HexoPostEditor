#ifndef HPEABOUTDIALOG_H
#define HPEABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class HPEAboutDialog;
}

class HPEAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HPEAboutDialog(QWidget *parent = nullptr);
    ~HPEAboutDialog();

private:
    Ui::HPEAboutDialog *ui;
};

#endif // HPEABOUTDIALOG_H
