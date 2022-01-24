#include "hpeaboutdialog.h"
#include "ui_hpeaboutdialog.h"

#include <QPixmap>

HPEAboutDialog::HPEAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HPEAboutDialog)
{
    ui->setupUi(this);

    this->resize(575, 375);
    this->setFixedSize(575, 375);
    setModal(true);
    setWindowFlags(Qt::Dialog);
    setWindowTitle("About");

    ui->iconLabel->setPixmap(
        QPixmap(":/icons/resources/icons/Icon-Tomortec.png")
            .scaled(QSize(150, 150),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation)
    );

    ui->descriptionLabel->setTextFormat(Qt::RichText);
    ui->descriptionLabel->setText(QString(
        "<h1>Version %1 </h1>"
        "Hexo Post Editor is a free software.<br>"
        "Hexo Post Editor is released under GPL v3.<br>"
        "The repository of Hexo Post Editor is <br>"
        "<a href='https://github.com/Tomortec/HexoPostEditor'>https://github.com/Tomortec/HexoPostEditor</a><br>"
        "<br>"
        "<h1>Contributors</h1>"
        "@<a href='https://github.com/LMLAZ'>LeoZhao</a> and @<a href='https://github.com/TomortecAlbert'>AlbertChen</a>"
        " from Tomortec<br>"
        "Contact us: <a href='mailto:everything@tomortec.com'>everything@tomortec.com</a><br>"
        "Visit our Homepage: <a href='https://tomortec.com/'>https://tomortec.com</a><br>"
        "Copyright © 2021-2022 Tomortec"
    ).arg(QCoreApplication::applicationVersion()));
}

HPEAboutDialog::~HPEAboutDialog()
{
    delete ui;
}
