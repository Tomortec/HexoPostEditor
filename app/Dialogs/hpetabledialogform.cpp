#include "hpetabledialogform.h"
#include "ui_hpetabledialogform.h"

int HPETableDialogForm::maxLengthText = 0;

HPETableDialogForm::HPETableDialogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPETableDialogForm)
{
    ui->setupUi(this);

    connect(ui->rowCountStepper, &QSpinBox::valueChanged, ui->tableWidget,
            [this](int row){ ui->tableWidget->setRowCount(row); });
    connect(ui->colCountStepper, &QSpinBox::valueChanged, ui->tableWidget,
            [this](int col){ ui->tableWidget->setColumnCount(col); });
}

HPETableDialogForm::~HPETableDialogForm()
{
    delete ui;
}

QList<QList<QString> > HPETableDialogForm::getTableContent() const
{
    QList<QStringList > result;
    int row = ui->tableWidget->rowCount();
    int col = ui->tableWidget->columnCount();
    for(int r = 0; r < row; ++r)
    {
        QStringList tmp;
        for(int c = 0; c < col; ++c)
            if(ui->tableWidget->item(r,c))
            {
                tmp.append(ui->tableWidget->item(r, c)->text());
                HPETableDialogForm::maxLengthText = qMax(HPETableDialogForm::maxLengthText, tmp.last().length());
            }
            else
                tmp.append("");
        result.append(tmp);
    }
    return result;
}

void HPETableDialogForm::resetTable()
{
    ui->tableWidget->clear();
}
