#include "imginfodialog.h"
#include "ui_imginfodialog.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QTime>
#include <QDebug>

imgInfoDialog::imgInfoDialog(QWidget *parent, QString globalImgPath, QString tmpAbPath) :
    QDialog(parent),
    ui(new Ui::imgInfoDialog)
{
    m_globalImgPath = globalImgPath;
    m_tmpAbPath = tmpAbPath;
    QFileInfo info(m_globalImgPath);
    if(!info.exists())
    {
        QMessageBox::warning(this,tr("警告"),tr("文件不存在！"));
        close();
    }
    ui->setupUi(this);
    //加载预览图像
    QImage img(m_globalImgPath);
    int w = img.width();
    int h = img.height();
    img = img.scaled(ui->graphicsView->width(),ui->graphicsView->height(),Qt::KeepAspectRatio);
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0,0,img.width(),img.height());
    scene->addPixmap(QPixmap::fromImage(img));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //判断是否为临时文件
    isTmpFile = (info.path() == m_tmpAbPath);
    qDebug()<<"info.path = "<<info.path()<<"   "<<isTmpFile;
    //加载图像信息
    ui->lab_imgDepth->setText(QString::number(img.depth()));
    ui->lab_imgData->setText(makeImgData(info));
    ui->lab_imgSize->setText(QString::number(w)+" * "+QString::number(h));
    //根据isTmpFile写入不同图像信息
    if(isTmpFile)
    {
        ui->lab_imgKind->setText(tr("临时文件"));
        ui->lab_imgName->setText("--");
        ui->lab_imgFormat->setText("--");
        ui->lab_imgAbPath->setText("--");
        ui->lab_imgCreateTime->setText("--");
    }
    else
    {
        ui->lab_imgKind->setText(tr("本地文件"));
        ui->lab_imgName->setText(info.fileName().remove("."+info.suffix()));
        ui->lab_imgFormat->setText(info.suffix());
        ui->lab_imgAbPath->setText(info.absoluteFilePath());
        ui->lab_imgCreateTime->setText(info.created().toString("yyyy-MM-dd hh:mm:ss"));
    }
}

imgInfoDialog::~imgInfoDialog()
{
    delete ui;
}

QString imgInfoDialog::makeImgData(QFileInfo info)
{
    double data = static_cast<double>(info.size());
    if(data <= 1024.0)
        return QString::number(data, 'f', 0).toUpper() + "B";
    else if(data <= 1048576.0)
        return QString::number(data / 1024, 'f', 2).toUpper() + "KB";
    else
        return QString::number(data / 1024 / 1024, 'f', 2).toUpper() + "MB";
}
