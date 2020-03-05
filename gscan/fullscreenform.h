#ifndef FULLSCREENFORM_H
#define FULLSCREENFORM_H

#include <QDialog>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "mygraphicsscene.h"

namespace Ui {
class fullScreenForm;
}

class MainWindow;
class fullScreenForm : public QDialog
{
    Q_OBJECT

public:
    explicit fullScreenForm(QWidget *parent = nullptr);
    ~fullScreenForm();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent* event);

private slots:
    void on_pbtn_prePage_clicked();
    void on_pbtn_nextPage_clicked();
    void hideBtn();//隐藏计时timeout响应槽，启动淡化按钮
    void btnFade();//淡化计时timeout响应槽，逐步淡化按钮
    void subWinMouseMoved();//响应graphicsScene鼠标移动事件
private:
    Ui::fullScreenForm *ui;
    MainWindow* mainW;//MainWindow指针
    QImage srcImg;//来源图像副本
    MyGraphicsScene *src_pic;//全屏画布
    int index;//当前图像在MainWindow列表中的序号
    QStringList namesList;//MainWindow列表中所有图像的文件路径
    QTimer timer_hide,timer_fade;//按钮隐藏计时器，按钮淡化计时器   
	QGraphicsOpacityEffect *effect1,*effect2;//按钮对应的淡化效果   
	void setBtnVisible(int index);//根据当前图像序号刷新按钮可见情况
    void setBtnOpacity(qreal opacity);//设置按钮透明度   
	void freshImg();//刷新画布图像
    void initWindow();//初始化全屏窗口
};

#endif // FULLSCREENFORM_H
