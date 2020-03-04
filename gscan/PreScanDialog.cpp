#include "PreScanDialog.h"
#include "ui_PreScanDialog.h"

PreScanDialog::PreScanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreScanDialog)
{
    ui->setupUi(this);
    ui->graphicsView_front->setScene(&frontScene);
    ui->graphicsView_back->setScene(&backScene);
}

PreScanDialog::~PreScanDialog()
{
    delete ui;
}

void PreScanDialog::setImages(QString img_front, QString img_back)
{
    ui->graphicsView_front->setFixedSize(QSize(446,630));
    ui->graphicsView_back->setFixedSize(QSize(446,630));
    frontImg = QImage(img_front);
    backImg = QImage(img_back);
    frontScene.setImage(&frontImg,ui->graphicsView_front->width()-3,ui->graphicsView_front->height()-3);
    backScene.setImage(&backImg,ui->graphicsView_back->width()-3,ui->graphicsView_back->height()-3);
}
