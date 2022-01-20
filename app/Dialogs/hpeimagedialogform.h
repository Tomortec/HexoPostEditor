#ifndef HPEIMAGEDIALOGFORM_H
#define HPEIMAGEDIALOGFORM_H

#include <QWidget>
#include <QGroupBox>

namespace Ui {
class HPEImageDialogForm;
}

class HPEMainWindow;

class HPEImageDialogForm : public QWidget
{
    Q_OBJECT

public:
    explicit HPEImageDialogForm(QWidget *parent = nullptr);
    ~HPEImageDialogForm();

private:
    Ui::HPEImageDialogForm *ui;
    QString m_imagePath;
    HPEMainWindow* m_mainWindow;

public:
    QString getImageFilePath() const;

    void resetEdit();

protected:
    void showEvent(QShowEvent*) override;

private:
    void beautifyGroupBox(QGroupBox*);
private slots:
    void onOpenFileDialog();

signals:
    void imageSelected();
};

#endif // HPEIMAGEDIALOGFORM_H
