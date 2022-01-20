#ifndef HPEHEADINGDIALOGFORM_H
#define HPEHEADINGDIALOGFORM_H

#include <QWidget>

namespace Ui {
class HPEHeadingDialogForm;
}

class HPEHeadingDialogForm : public QWidget
{
    Q_OBJECT

public:
    explicit HPEHeadingDialogForm(QWidget *parent = nullptr);
    ~HPEHeadingDialogForm();

private:
    Ui::HPEHeadingDialogForm *ui;
    bool m_addHorizontalRule = false;
    QString m_headingType = "# ";

public:
    // with headingType (etc. "# ", "## ")
    QString getHeadingText() const;
    bool isAddHorizontalRule() const;

    void resetEdit();
};

#endif // HPEHEADINGDIALOGFORM_H
