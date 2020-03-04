#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsScene>

class MainWindow;

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicsScene();
    ~MyGraphicsScene();
    void setImage(QImage *src_img, int width, int height);//获取图像来源并加载图像    
	void zoomIn();
    void zoomOut();

signals:
    void mouseMoved();

protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QImage m_img;//源图像的副本
    MainWindow* mainW;
    void freshDisplayImg(int width,int height);//刷新GraphicsScene上的图像
    void on_wheel_RotatePos();//鼠标向前滚动操作
    void on_wheel_RotateNeg();//鼠标向后滚动操作
};

#endif // MYGRAPHICSVIEW_H
