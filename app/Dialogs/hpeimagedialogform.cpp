#include "hpeimagedialogform.h"
#include "ui_hpeimagedialogform.h"

#include <QGroupBox>
#include <QPixmap>
#include <QFileDialog>

#include "hpemainwindow.h"

HPEImageDialogForm::HPEImageDialogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPEImageDialogForm)
{
    ui->setupUi(this);

    ui->imageList->setIconSize(QSize(100, 100));

    foreach(QWidget* widget, qApp->topLevelWidgets())
    {
        m_mainWindow = qobject_cast<HPEMainWindow*>(widget);
        if(m_mainWindow)
            break;
    }

    connect(ui->addedImagesGroup, &QGroupBox::toggled, ui->newSourceGroup, [this](bool flag){
        ui->newSourceGroup->setChecked(!flag);
        beautifyGroupBox(ui->addedImagesGroup);
        beautifyGroupBox(ui->newSourceGroup);
    });
    connect(ui->newSourceGroup, &QGroupBox::toggled, ui->addedImagesGroup, [this](bool flag){
        ui->addedImagesGroup->setChecked(!flag);
        beautifyGroupBox(ui->addedImagesGroup);
        beautifyGroupBox(ui->newSourceGroup);
    });
    connect(ui->openFileButton, &QPushButton::clicked, this, &HPEImageDialogForm::onOpenFileDialog);
    connect(ui->imageList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item){
        item->setSelected(true);
        //getImageFilePath();
        emit imageSelected();
    });

    //TODO: add to setting
    ui->newSourceGroup->setChecked(false);
}

HPEImageDialogForm::~HPEImageDialogForm()
{
    delete ui;
}

QString HPEImageDialogForm::getImageFilePath() const
{
    //should copy to resource dir and reset path
    if(ui->newSourceGroup->isChecked())
    {
        QFileInfo currentFileInfo(m_mainWindow->getFilePath());
        QDir currentDir = currentFileInfo.absoluteDir();
        if(!currentDir.cd(currentFileInfo.baseName()))
        {
            //error
            return ui->pathEdit->text();
        }
        QFileInfo targetFileInfo(ui->pathEdit->text());
        if(!QFile::copy(ui->pathEdit->text(), currentDir.absoluteFilePath(targetFileInfo.fileName())))
            return ui->pathEdit->text();
        return targetFileInfo.fileName();
    }
    else
    {
        if(!ui->imageList->selectedItems().empty())
        {
            QListWidgetItem* selected = ui->imageList->selectedItems().first();
            if(QFileInfo(selected->toolTip()).absolutePath().endsWith("/images"))
                return QString("/images/%1").arg(selected->text());
            else
                return selected->text();
        }
        else
            return "";
    }
}

void HPEImageDialogForm::resetEdit()
{
    if(ui->newSourceGroup->isChecked())
        ui->pathEdit->setText("");
    m_imagePath = "";
}

void HPEImageDialogForm::showEvent(QShowEvent *)
{
    QList<QFileInfo> imageFiles;
    QFileInfo currentFileInfo(m_mainWindow->getFilePath());
    QDir currentDir = currentFileInfo.absoluteDir();
    if(currentDir.cd(currentFileInfo.baseName()))
        imageFiles.append(currentDir.entryInfoList(QStringList() << "*.jpeg" << "*.jpg" << "*.png" << "*.tiff",
                                               QDir::Files));
    QDir sourceDir = currentFileInfo.absoluteDir();
    if(sourceDir.dirName() == "source" || (sourceDir.cdUp() && sourceDir.dirName() == "source"))
        if(sourceDir.cd("images"))
            imageFiles.append(sourceDir.entryInfoList(QStringList() << "*.jpeg" << "*.jpg" << "*.png" << "*.tiff",
                                                   QDir::Files));

    ui->imageList->clear();
    for(const QFileInfo& imageInfo : imageFiles)
    {
        QListWidgetItem* item = new QListWidgetItem(ui->imageList);
        item->setIcon(QIcon(QPixmap(imageInfo.absoluteFilePath()).scaled(
                                QSize(100, 100), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        item->setText(imageInfo.fileName());
        item->setToolTip(imageInfo.absoluteFilePath());
        ui->imageList->addItem(item);
    }
}

void HPEImageDialogForm::beautifyGroupBox(QGroupBox *target)
{
    if(target->isChecked())
        target->setStyleSheet("color: gray; QGroupBox{ border: 4px solid rgb(208, 208, 208); }");
    else
        target->setStyleSheet("color: rgb(235, 235, 235); QGroupBox{ border: 4px solid rgb(235, 235, 235); }");
}

void HPEImageDialogForm::onOpenFileDialog()
{
    m_imagePath = QFileDialog::getOpenFileName(this, tr("Select an image file"),
                      "", tr("Images (*.jpeg *.jpg *.png *.tiff);;JPEG (*.jpeg);;JPG (*.jpg);;PNG (*.png);;TIF (*.tif);;TIFF (*.tiff)"));
    if(m_imagePath.isEmpty())
        return;
    ui->pathEdit->setText(m_imagePath);
}
