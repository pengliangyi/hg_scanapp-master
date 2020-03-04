#include "fullscreenform.h"
#include "ui_fullscreenform.h"
#include "mainwindow.h"
#include <QDesktopWidget>
#include <QGraphicsEffect>
#include <QKeyEvent>
#include <QDebug>

fullScreenForm::fullScreenForm(QWidget *parent) :
    QDialog(0),
    src_pic(new MyGraphicsScene),
    effect1(new QGraphicsOpacityEffect),
    effect2(new QGraphicsOpacityEffect),
    mainW(dynamic_cast<MainWindow*>(parent)),
    ui(new Ui::fullScreenForm)
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowState(Qt::WindowFullScreen);
    ui->setupUi(this);
    connect(src_pic,SIGNAL(mouseMoved()),this,SLOT(subWinMouseMoved()));
    connect(&timer_hide,SIGNAL(timeout()),this,SLOT(hideBtn()));
    connect(&timer_fade,SIGNAL(timeout()),this,SLOT(btnFade()));
    index = mainW->getListWidgetCurrIndex();
    namesList = mainW->getListWidgetFileNames();
    initWindow();
    freshImg();
    QTimer::singleShot(2000,this,SLOT(hideBtn()));
}

fullScreenForm::~fullScreenForm()
{
    delete effect1;
    delete effect2;
    effect1 = nullptr;
    effect2 = nullptr;
    delete ui;
}
//events (3)
void fullScreenForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        close();
    }
}
void fullScreenForm::mouseMoveEvent(QMouseEvent *event)
{
    subWinMouseMoved();
}
void fullScreenForm::wheelEvent(QWheelEvent *event)
{
    if(event->delta()<0)
    {
        src_pic->zoomOut();
    }
    else if(event->delta()>0)
    {
        src_pic->zoomIn();
    }
}
//func (4)
void fullScreenForm::initWindow()
{
    //����src������С�Ͱ�ťλ��   
	QDesktopWidget desk;
    QRectF screenRect = desk.screenGeometry();
    ui->graphicsView->setFixedSize(screenRect.width(),screenRect.height());
    ui->pbtn_prePage->setGeometry(50,screenRect.height()/2,ui->pbtn_prePage->width(),ui->pbtn_prePage->height());
    ui->pbtn_nextPage->setGeometry(screenRect.width()-ui->pbtn_nextPage->width()-50,screenRect.height()/2,ui->pbtn_nextPage->width(),ui->pbtn_nextPage->height());
    ui->graphicsView->setScene(src_pic);
}
void fullScreenForm::setBtnVisible(int index)
{
    if(mainW->getListWidgetCount() == 1)
    {
        ui->pbtn_prePage->setVisible(false);
        ui->pbtn_nextPage->setVisible(false);
    }
    else if(index == 0)
    {
        ui->pbtn_prePage->setVisible(false);
        ui->pbtn_nextPage->setVisible(true);
    }
    else if(index == mainW->getListWidgetCount()-1)
    {
        ui->pbtn_nextPage->setVisible(false);
        ui->pbtn_prePage->setVisible(true);
    }
    else
    {
        ui->pbtn_prePage->setVisible(true);
        ui->pbtn_nextPage->setVisible(true);
    }
    setBtnOpacity(0.7);//�ָ���ť͸����Ϊ0.7
}
void fullScreenForm::setBtnOpacity(qreal opacity)
{
    effect1->setOpacity(opacity);
    ui->pbtn_prePage->setGraphicsEffect(effect1);
    effect2->setOpacity(opacity);
    ui->pbtn_nextPage->setGraphicsEffect(effect2);
}
void fullScreenForm::freshImg()
{
    QDesktopWidget desk;
    QRect screenRect = desk.screenGeometry();
    src_pic->clear();
    srcImg = QImage(namesList.at(index));
    QImage img = srcImg;
    img = img.scaled(screenRect.size(), Qt::KeepAspectRatio);//��ͼ�����ŵ�������С
    src_pic->setImage(&srcImg,img.width(),img.height());
    setBtnVisible(index);
}

//slots (4)
void fullScreenForm::on_pbtn_prePage_clicked()
{
    index--;
    freshImg();
}
void fullScreenForm::on_pbtn_nextPage_clicked()
{
    index++;
    freshImg();
}
void fullScreenForm::hideBtn()
{
    timer_fade.start(10);
}
void fullScreenForm::btnFade()
{
    if((effect1->opacity() == 0.0)&&(effect2->opacity() == 0.0))
    {
        timer_fade.stop();
        ui->pbtn_prePage->setVisible(false);
        ui->pbtn_nextPage->setVisible(false);
        return;
    }
    qreal i = effect1->opacity();//���а�ťͳһ��������ȡ����effect͸����   
	setBtnOpacity(i-0.005);
}

void fullScreenForm::subWinMouseMoved()
{
    setBtnVisible(index);//���ݵ�ǰͼ��index��ʾ��ť
    timer_fade.stop();//�жϵ�����ʱ
    timer_hide.setSingleShot(true);//���ð�ť��ʾ��ʱΪ���δ���ģʽ   
	timer_hide.start(1000);//���ð�ť1���ʼ����
}


