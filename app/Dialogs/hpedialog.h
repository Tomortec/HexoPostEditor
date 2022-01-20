#ifndef HPEDIALOG_H
#define HPEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>

namespace Ui {
class HPEDialog;
}

class HPEDialog : public QDialog
{
    Q_OBJECT

public:
    enum DialogType
    {
        HEADING,
        LINK,
        IMAGE,
        CODE,
        TABLE
    };

public:
    explicit HPEDialog(DialogType type, QWidget *parent = nullptr);
    ~HPEDialog();

private:
    Ui::HPEDialog *ui;
    DialogType m_type;
    QWidget* m_centralWidget;
    QHBoxLayout* m_centralWidgetLayout;

    QString m_result;

private:
    void handleHeadingData();
    void handleLinkData();
    void handleImageData();
    void handleCodesData();
    void handleTableData();

signals:
    void dialogConfirm(const QString& result);
    void dialogCancel();

private slots:
    void onConfirm();
    void onCancel();

};

#endif // HPEDIALOG_H
