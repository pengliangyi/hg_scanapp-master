#include "localbcdialog.h"
#include "ui_localbcdialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QDebug>


LocalBCDialog::LocalBCDialog(QWidget *parent, QImage *src, QString appDirPath) :
    QDialog(parent),
    dst_pic(new QGraphicsScene),
    dstImg(nullptr),
    ui(new Ui::LocalBCDialog)
{
    ui->setupUi(this);
    this->setFixedSize(720,510);
    ui->graphicsView_src->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_src->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_dst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_dst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_appDirPath = appDirPath;

    srcImg = src->depth() == 32 ? src->convertToFormat(QImage::Format_RGB888) : *src;
    if(srcImg.isNull())
    {
        QMessageBox::warning(this,tr("警告 "),tr("加载图像失败！ "));
        close();
    }
    srcImg = srcImg.scaled(imgSizeWidth,imgSizeHeight,Qt::KeepAspectRatio);
    //create src scene and image
	QGraphicsScene *src_pic = new QGraphicsScene;
    ui->graphicsView_src->setScene(src_pic);
    src_pic->addPixmap(QPixmap::fromImage(srcImg));
    //load dst scene and image
	ui->graphicsView_dst->setScene(dst_pic);
    dst_pic->addPixmap(QPixmap::fromImage(srcImg));
}

LocalBCDialog::~LocalBCDialog()
{
    delete ui;
}


void LocalBCDialog::freshDstImg(int alpha, int beta)
{
    if (dstImg != nullptr) delete dstImg;
    dstImg = new QImage(srcImg);
    //transform by pixel
    int total = srcImg.byteCount();
    uchar* ptr_src = srcImg.bits();
    uchar* ptr_dst = dstImg->bits();
    for (int i = 0; i < total; i++)
    {
        if (ptr_src[i] > 127)
            ptr_dst[i] = qMin(255, qMax((ptr_src[i] + alpha*2), 128));
        else
            ptr_dst[i] = qMin(127, qMax((ptr_src[i] - alpha*2), 0));
        ptr_dst[i] = qMin(255, qMax(ptr_dst[i] + beta, 0));
    }
    //show dst image preview
    dst_pic->addPixmap(QPixmap::fromImage(*dstImg));
}

void LocalBCDialog::on_Slider_brightness_sliderMoved(int position)
{
    freshDstImg(ui->Slider_contrast->value()-31,position-128);
}

void LocalBCDialog::on_Slider_contrast_sliderMoved(int position)
{
    freshDstImg(position-31,ui->Slider_brightness->value()-128);
}

void LocalBCDialog::on_buttonBox_accepted()
{
    if(ui->Slider_brightness->value()==128 && ui->Slider_contrast->value()==31)
    {
        this->setResult(QDialog::Rejected);
        close();
        return;
    }
    int alpha = ui->Slider_contrast->value()-31;
    int beta = ui->Slider_brightness->value()-128;
    //load origin image
    QImage originImg(dynamic_cast<MainWindow*>(parent())->globalImgPath);
    if (dstImg != nullptr) delete dstImg;
    dstImg = new QImage(originImg);
    //transform by pixel
    int total = originImg.byteCount();
    uchar* ptr_src = originImg.bits();
    uchar* ptr_dst = dstImg->bits();
    for (int i = 0; i < total; i++)
    {
        if (ptr_src[i] > 127)
            ptr_dst[i] = qMin(255, qMax((ptr_src[i] + alpha*2), 128));
        else
            ptr_dst[i] = qMin(127, qMax((ptr_src[i] - alpha*2), 0));
        ptr_dst[i] = qMin(255, qMax(ptr_dst[i] + beta, 0));
    }
    //create tmp copy file 
#if 1
    int index = 1;
    QString copyPath = m_appDirPath+"/tmp/bcAdjustCopy"+QString::number(index)+".jpg";
    while(QFile::exists(copyPath))
    {
        index++;
        copyPath = m_appDirPath+"/tmp/bcAdjustCopy"+QString::number(index)+".jpg";
    }
#else
    QString copyPath = m_appDirPath+"/tmp/bcAdjustCopy.jpg";
#endif
    dstImg->save(copyPath);//save tmp copy file
}

void LocalBCDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) != QDialogButtonBox::Reset)
        return;
    ui->Slider_brightness->setValue(128);
    ui->Slider_contrast->setValue(31);
    if (dstImg != nullptr) delete dstImg;
    dstImg = new QImage(srcImg);
    dst_pic->addPixmap(QPixmap::fromImage(*dstImg));
}
