#include "hpedialog.h"
#include "ui_hpedialog.h"

#include <QMessageBox>
#include <QUrl>

#include "hpeheadingdialogform.h"
#include "hpelinkdialogform.h"
#include "hpeimagedialogform.h"
#include "hpecodesdialogform.h"
#include "hpetabledialogform.h"

HPEDialog::HPEDialog(HPEDialog::DialogType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HPEDialog),
    m_type(type)
{
    ui->setupUi(this);
    if(!ui->centralWidget->layout())
    {
        m_centralWidgetLayout = new QHBoxLayout(ui->centralWidget);
        ui->centralWidget->setLayout(m_centralWidgetLayout);
    }

    this->setWindowFlag(Qt::Dialog);

    switch(m_type)
    {
    case HEADING:
        this->setWindowTitle("Adding a Heading");
        m_centralWidget = new HPEHeadingDialogForm(ui->centralWidget);
        ui->centralWidget->layout()->addWidget(m_centralWidget);
        break;
    case LINK:
        this->setWindowTitle("Adding Link");
        m_centralWidget = new HPELinkDialogForm(ui->centralWidget);
        ui->centralWidget->layout()->addWidget(m_centralWidget);
        break;
    case IMAGE:
        this->setWindowTitle("Adding Images");
        m_centralWidget = new HPEImageDialogForm(ui->centralWidget);
        ui->centralWidget->layout()->addWidget(m_centralWidget);
        connect(qobject_cast<HPEImageDialogForm*>(m_centralWidget), &HPEImageDialogForm::imageSelected,
                this, &HPEDialog::onConfirm);
        break;
    case CODE:
        this->setWindowTitle("Adding Code blocks");
        m_centralWidget = new HPECodesDialogForm(ui->centralWidget);
        ui->centralWidget->layout()->addWidget(m_centralWidget);
        break;
    case TABLE:
        this->setWindowTitle("Adding a Table");
        m_centralWidget = new HPETableDialogForm(ui->centralWidget);
        ui->centralWidget->layout()->addWidget(m_centralWidget);
        break;
    default:
        break;
    }

    connect(ui->confirmButton, &QPushButton::clicked, this, &HPEDialog::onConfirm);
    connect(ui->cancelButton,  &QPushButton::clicked, this, &HPEDialog::onCancel);
}

HPEDialog::~HPEDialog()
{
    delete ui;
}

void HPEDialog::handleHeadingData()
{
    HPEHeadingDialogForm* centralWidget = ui->centralWidget->findChild<HPEHeadingDialogForm*>();
    if(!centralWidget)
        return;

    m_result = "\n" + centralWidget->getHeadingText() +
                (centralWidget->isAddHorizontalRule()? "\n***\n" : "");

    centralWidget->resetEdit();
}

void HPEDialog::handleLinkData()
{
    HPELinkDialogForm* centralWidget = ui->centralWidget->findChild<HPELinkDialogForm*>();
    if(!centralWidget)
        return;

    QUrl validator(centralWidget->getLinkSite());
    if(!validator.isValid())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                                             tr("this may be an Invalid Link, continue anyway?"));
        if(button != QMessageBox::Yes)
            return;
    }

    m_result = QString("[%1](%2)").arg(centralWidget->getLinkName(), centralWidget->getLinkSite());

    centralWidget->resetEdits();
}

void HPEDialog::handleImageData()
{
    HPEImageDialogForm* centralWidget = ui->centralWidget->findChild<HPEImageDialogForm*>();
    if(!centralWidget)
        return;

    m_result = QString("![](%1)").arg(centralWidget->getImageFilePath());

    centralWidget->resetEdit();
}

void HPEDialog::handleCodesData()
{
    HPECodesDialogForm* centralWidget = ui->centralWidget->findChild<HPECodesDialogForm*>();
    if(!centralWidget)
        return;

    m_result = QString("```%1\n%2\n```").arg(centralWidget->getLanguage(), centralWidget->getCodes());

    centralWidget->resetEdit();
}

void HPEDialog::handleTableData()
{
    HPETableDialogForm* centralWidget = ui->centralWidget->findChild<HPETableDialogForm*>();
    if(!centralWidget)
        return;

    QList<QStringList> tableContents(centralWidget->getTableContent());
    const int fieldWidth = qMax(HPETableDialogForm::maxLengthText, QString("HEADER").length());

    int row = tableContents.length();   //the least is 1
    int col = tableContents[0].length();//the least is 1
    QString header = "|" + QString("%1|").arg("HEADER", -1*fieldWidth).repeated(col) + "\n"
                   + "|" + QString("%1|").arg(QString("-").repeated(fieldWidth)).repeated(col) + "\n";
    m_result = header;
    for(int r = 0; r < row; ++r)
    {
        QString tmp("|");
        for(int c = 0; c < col; ++c)
            tmp += QString("%1|").arg(tableContents[r][c], -1*fieldWidth);
        tmp += "\n";
        m_result += tmp;
    }

    centralWidget->resetTable();
}

void HPEDialog::onConfirm()
{
    ui->confirmButton->setFocus();
    switch (m_type)
    {
    case HEADING:
        handleHeadingData();
        break;
    case LINK:
        handleLinkData();
        break;
    case IMAGE:
        handleImageData();
        break;
    case CODE:
        handleCodesData();
        break;
    case TABLE:
        handleTableData();
        break;
    default:
        return;
    }
    emit dialogConfirm(m_result);
    accept();
}

void HPEDialog::onCancel()
{
    emit dialogCancel();
    reject();
}
