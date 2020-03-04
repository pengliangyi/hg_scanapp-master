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
    void initAllLstPnt();   //��ʼ��������ɫ����
    void setLstPnt_RGB(const QList<QPoint>& plst);  //��������RGB���ߵĹؼ��㣨<=4)
    QList<QPoint> getLstPnt_RGB()const; //��ȡ����RGB���ߵĹؼ���
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

    void setHist_RGB(const QVector<int>& hist);//����RGB�ı���ֱ��ͼ
    QVector<int> getHist_RGB()const;//��ȡRGB�ı���ֱ��ͼ
    void setHist_RED(const QVector<int>& hist);
    QVector<int> getHist_RED()const;
    void setHist_BLUE(const QVector<int>& hist);
    QVector<int> getHist_BLUE()const;
    void setHist_GREEN(const QVector<int>& hist);
    QVector<int> getHist_GREEN()const;
    void setHist_GRAY(const QVector<int>& hist);
    QVector<int> getHist_GRAY()const;

    void getCurLineLUT(uchar* table, size_t length = 256);//��ȡ��ǰ�����ϵ����е㲢����table
    QVector<unsigned char> getCurLinePntVec();//��ȡ��ǰ�����ϵ����е�
    void setXCoorVal(const int &xVal);//���ñ�ѡ�еĵ��X����
    void setYCoorVal(const int &yVal);//���ñ�ѡ�еĵ��Y����
    void updateCurLinePnt(const int& colType);//���ݵ�ǰ��ѡ��ɫ��colType���ػ����
    void initSelectColLine(const int& colType);//��ʼ����ǰ��ѡ��ɫ��colType�����������
    void SetAllLinePnt(QVector<int>& all);//�����Զ������ߵ����е�

signals:
    void dragPointChecked(bool);//�����ϵĵ㱻ѡ�е��ź�
    void mouseLeaveSig();//����뿪�ؼ����ź�
    void mouseCoordSig(QPoint pos);//���������ź�
    void lineChangeSig();

private:
    void initInterface();//��ʼ������
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void drawCoordinate(QPainter&);//��ֱ������
    void drawBackgroudColor(QPainter&,QVector<int>&);//������ֱ��ͼ
    void drawBackColorBySlc(QPainter&);//����ͬ�ı���ֱ��ͼ
    void drawCoorScale(QPainter&);//�����߿̶�
    void drawLineFromPoint(QList<QPoint> &plst, const int &col);//���ݶ��������߷��̣�����������
    void drawAllPoint(QPainter&);//�����������ߵĹؼ���
    void drawCurveByColor(QPainter&);//����ͬ����ɫ����
    void drawLineByVector(QPainter&,QVector<int>&);//����������е����ӻ�������
    int caculateAllMi(int num,int n);//����num��n�η���ֵ

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
