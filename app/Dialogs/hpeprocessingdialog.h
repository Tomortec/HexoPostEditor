#ifndef HPEPROCESSINGDIALOG_H
#define HPEPROCESSINGDIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class HPEProcessingDialog;
}

class HPEProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HPEProcessingDialog(const QString& prompt = "", QWidget *parent = nullptr);
    ~HPEProcessingDialog();

    void setPrompt(const QString&);

private:
    Ui::HPEProcessingDialog *ui;
    QMovie* m_movie;

    void moveToCenter();

public slots:
    void showError(const QString&);
    void accept() override;

protected:
    void showEvent(QShowEvent*) override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // HPEPROCESSINGDIALOG_H
