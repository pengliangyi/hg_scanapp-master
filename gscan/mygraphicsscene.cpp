#include "mygraphicsscene.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>

MyGraphicsScene::MyGraphicsScene()
{
}

MyGraphicsScene::~MyGraphicsScene()
{
}
//events (1)
void MyGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    if(this->items(Qt::AscendingOrder).count() == 0) return;//检查当前画布上是否存在图像
    if(wheelEvent->delta()<0)
    {
        on_wheel_RotateNeg();
    }
    else if(wheelEvent->delta()>0)
    {
        on_wheel_RotatePos();
    }
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit mouseMoved();
}

//slots (2)
void MyGraphicsScene::on_wheel_RotatePos()
{
    if(m_img.isNull()) return;
    double step = 1.2;
    double h = this->height();
    double w = this->width();
    if((h >= m_img.height()*4.0) || (w >= m_img.width()*4.0))
    {
        return;
    }
    else
    {
        freshDisplayImg(w*step,h*step);
    }
}
void MyGraphicsScene::on_wheel_RotateNeg()
{
    if(m_img.isNull()) return;
    double step = 0.8;
    double h = this->height();
    double w = this->width();
    if((h <= m_img.height()*0.1) || (w <= m_img.width()*0.1))
    {
        return;
    }
    else
    {
        freshDisplayImg(w*step,h*step);
    }
}
//private func (1)
void MyGraphicsScene::freshDisplayImg(int width,int height)
{
    if(m_img.isNull()) return;
    this->clear();
    QImage img = m_img;
    img = img.scaled(width,height,Qt::KeepAspectRatio);
    this->setSceneRect(0,0,img.width(),img.height());
    this->addPixmap(QPixmap::fromImage(img));
}
//public func (3)
void MyGraphicsScene::setImage(QImage *src_img,int width,int height)
{
    if(src_img->isNull()) return;
    m_img = *src_img;
    freshDisplayImg(width,height);
}
void MyGraphicsScene::zoomIn()
{
    on_wheel_RotatePos();
}
void MyGraphicsScene::zoomOut()
{
    on_wheel_RotateNeg();
}
