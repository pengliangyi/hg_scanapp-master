#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "colorlinesetdef.h"

namespace Ui {
class Widget;
}

enum COLOR_TPYE
{
    RGB,
    GRAY,
    RED,
    BLUE,
    GREEN
};

class  Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void initAllLstPnt();   //初始化所有颜色曲线
    void setLstPnt_RGB(const QList<QPoint>& plst);  //设置生成RGB曲线的关键点（<=4)
    QList<QPoint> getLstPnt_RGB()const; //获取生成RGB曲线的关键点
    void setLstPnt_RED(const QList<QPoint>& plst);
    QList<QPoint> getLstPnt_RED()const;
    void setLstPnt_BLUE(const QList<QPoint>& plst);
    QList<QPoint> getLstPnt_BLUE()const;
    void setLstPnt_GREEN(const QList<QPoint>& plst);
    QList<QPoint> getLstPnt_GREEN()const;
    void setLstPnt_GRAY(const QList<QPoint>& plst);
    QList<QPoint> getLstPnt_GRAY()const;

    QVector<int> getRgbALLPoint();
    QList<QPoint> getRgbKeyPoint()const;
    void setRgbKeyPoint(const QList<QPoint>& pVec);
    QVector<int> getRedALLPoint();
    QList<QPoint> getRedKeyPoint()const;
    void setRedKeyPoint(const QList<QPoint>& pVec);
    QVector<int> getBlueALLPoint();
    QList<QPoint> getBlueKeyPoint()const;
    void setBlueKeyPoint(const QList<QPoint>& pVec);
    QVector<int> getGreenALLPoint();
    QList<QPoint> getGreenKeyPoint()const;
    void setGreenKeyPoint(const QList<QPoint>& pVec);
    QVector<int> getGrayALLPoint();
    QList<QPoint> getGrayKeyPoint()const;
    void setGrayKeyPoint(const QList<QPoint>& pVec);

    void setHist_RGB(const QVector<int>& hist);//设置RGB的背景直方图
    QVector<int> getHist_RGB()const;//获取RGB的背景直方图
    void setHist_RED(const QVector<int>& hist);
    QVector<int> getHist_RED()const;
    void setHist_BLUE(const QVector<int>& hist);
    QVector<int> getHist_BLUE()const;
    void setHist_GREEN(const QVector<int>& hist);
    QVector<int> getHist_GREEN()const;
    void setHist_GRAY(const QVector<int>& hist);
    QVector<int> getHist_GRAY()const;

    void getCurLineLUT(uchar* table, size_t length = 256);//获取当前曲线上的所有点并存入table
    QVector<unsigned char> getCurLinePntVec();//获取当前曲线上的所有点
    void setXCoorVal(const int &xVal);//设置被选中的点的X坐标
    void setYCoorVal(const int &yVal);//设置被选中的点的Y坐标
    void updateCurLinePnt(const int& colType);//根据当前所选颜色（colType）重绘界面
    void initSelectColLine(const int& colType);//初始化当前所选颜色（colType）界面的曲线
    void SetAllLinePnt(QVector<int>& all);//设置自定义曲线的所有点

signals:
    void dragPointChecked(bool);//曲线上的点被选中的信号
    void mouseLeaveSig();//鼠标离开控件的信号
    void mouseCoordSig(QPoint pos);//鼠标的坐标信号
    void lineChangeSig();

private:
    void initInterface();//初始化界面
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void drawCoordinate(QPainter&);//画直角坐标
    void drawBackgroudColor(QPainter&,QVector<int>&);//画背景直方图
    void drawBackColorBySlc(QPainter&);//画不同的背景直方图
    void drawCoorScale(QPainter&);//画虚线刻度
    void drawLineFromPoint(QList<QPoint> &plst, const int &col);//根据多点求出曲线方程，并画出曲线
    void drawAllPoint(QPainter&);//画出生成曲线的关键点
    void drawCurveByColor(QPainter&);//画不同的颜色曲线
    void drawLineByVector(QPainter&,QVector<int>&);//将数组的所有点连接绘制曲线
    int caculateAllMi(int num,int n);//计算num的n次方的值

private:
    Ui::Widget *ui;
    QVector<int> rgbBackColor;
    QVector<int> redBackColor;
    QVector<int> blueBackColor;
    QVector<int> greenBackColor;
    QVector<int> grayBackColor;
    QVector<int> rgbALLPoint;
    QVector<int> redALLPoint;
    QVector<int> blueALLPoint;
    QVector<int> greenALLPoint;
    QVector<int> grayALLPoint;
    QVector<int> allLinePntSet;
    QVector<unsigned char> linePoints;

    //QList<QPoint> points;
    QList<QPoint> pointLst;
    QList<QPoint> linePoint;
    QList<QPoint> greenLine;
    QList<QPoint> redLine;
    QList<QPoint> blueLine;
    QList<QPoint> rgbLine;
    QList<QPoint> grayLine;
    QColor color;

    unsigned char allLinePoints[256];
    bool leftMouseMv;
    bool newPoint;
    bool clickLine;
    int dragPoint;
    int selectCol;
};

#endif // WIDGET_H
