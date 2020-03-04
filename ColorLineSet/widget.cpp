#include "widget.h"
#include "ui_widget.h"
#include "gaosixy.h"
#include <iostream>
#include <QPainter>
#include <QVector>
#include <QDebug>
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("zuobiaozhou")); //设置标题栏标题
    //resize(300,300); //设置窗口初始大小
    //ui->table->setWindowFlags(Qt::WindowStaysOnTopHint);
    initAllLstPnt();
    initInterface();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setLstPnt_RGB(const QList<QPoint> &plst)
{
    rgbLine = QList<QPoint>(plst);
}

QList<QPoint> Widget::getLstPnt_RGB() const
{
    return rgbLine;
}

void Widget::setLstPnt_RED(const QList<QPoint> &plst)
{
    redLine = QList<QPoint>(plst);
}

void Widget::setLstPnt_BLUE(const QList<QPoint> &plst)
{
    blueLine = QList<QPoint>(plst);
}

QList<QPoint> Widget::getLstPnt_BLUE() const
{
    return blueLine;
}

void Widget::setLstPnt_GREEN(const QList<QPoint> &plst)
{
    greenLine = QList<QPoint>(plst);
}

QList<QPoint> Widget::getLstPnt_GREEN() const
{
    return greenLine;
}

void Widget::setLstPnt_GRAY(const QList<QPoint> &plst)
{
    grayLine = QList<QPoint>(plst);
}

QList<QPoint> Widget::getLstPnt_GRAY() const
{
    return grayLine;
}

QVector<int> Widget::getRgbALLPoint()
{
    return rgbALLPoint;
}

QList<QPoint> Widget::getRgbKeyPoint() const
{
    return rgbLine;
}

void Widget::setRgbKeyPoint(const QList<QPoint> &pLst)
{
    if(pLst.size() > 4) return;
    rgbLine = QList<QPoint>(pLst);
    pointLst.clear();
    pointLst = QList<QPoint>(rgbLine);
    drawLineFromPoint(rgbLine, RGB);
    update();
}

QVector<int> Widget::getRedALLPoint()
{
    return redALLPoint;
}

QList<QPoint> Widget::getRedKeyPoint() const
{
    return redLine;
}

void Widget::setRedKeyPoint(const QList<QPoint> &pLst)
{
    if(pLst.size() > 4) return;
    redLine = QList<QPoint>(pLst);
    drawLineFromPoint(redLine, RED);
    update();
}

QVector<int> Widget::getBlueALLPoint()
{
    return blueALLPoint;
}

QList<QPoint> Widget::getBlueKeyPoint() const
{
    return blueLine;
}

void Widget::setBlueKeyPoint(const QList<QPoint> &pLst)
{
    if(pLst.size() > 4) return;
    blueLine = QList<QPoint>(pLst);
    drawLineFromPoint(blueLine, BLUE);
    update();
}

QVector<int> Widget::getGreenALLPoint()
{
    return greenALLPoint;
}

QList<QPoint> Widget::getGreenKeyPoint() const
{
    return greenLine;
}

void Widget::setGreenKeyPoint(const QList<QPoint> &pLst)
{
    if(pLst.size() > 4) return;
    greenLine = QList<QPoint>(pLst);
    drawLineFromPoint(greenLine, GREEN);
    update();
}

QVector<int> Widget::getGrayALLPoint()
{
    return grayALLPoint;
}

QList<QPoint> Widget::getGrayKeyPoint() const
{
    return grayLine;
}

void Widget::setGrayKeyPoint(const QList<QPoint> &pLst)
{
    if(pLst.size() > 4) return;
    grayLine = QList<QPoint>(pLst);
    pointLst.clear();
    pointLst = QList<QPoint>(grayLine);
    drawLineFromPoint(grayLine, GRAY);
    update();
}

void Widget::initAllLstPnt()
{
    pointLst.clear();
    pointLst.append(QPoint(0,0));
    pointLst.append(QPoint(255,255));
    redLine.clear();
    redLine.append(QPoint(0,0));
    redLine.append(QPoint(255,255));
    blueLine.clear();
    blueLine.append(QPoint(0,0));
    blueLine.append(QPoint(255,255));
    greenLine.clear();
    greenLine.append(QPoint(0,0));
    greenLine.append(QPoint(255,255));
    rgbLine.clear();
    rgbLine.append(QPoint(0,0));
    rgbLine.append(QPoint(255,255));
    grayLine.clear();
    grayLine.append(QPoint(0,0));
    grayLine.append(QPoint(255,255));
    drawLineFromPoint(redLine, RED);
    drawLineFromPoint(rgbLine, RGB);
    drawLineFromPoint(greenLine, GREEN);
    drawLineFromPoint(blueLine, BLUE);
    drawLineFromPoint(grayLine, GRAY);
}

void Widget::initInterface()
{
    color = Qt::gray;
    selectCol = RGB;
    leftMouseMv = false;
    newPoint = false;
    clickLine = false;
    dragPoint = -1;
    setMouseTracking(true);
    ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
    ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                              "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                              "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true); //开启抗锯齿
    painter.translate(ui->widget->width(),ui->widget->height()); //坐标系统平移变换,把原点平移
    painter.scale(ui->table->width() / 258.0,ui->table->height() / 258.0); //坐标系统比例变换,使绘制的图形随窗口的放大而放大
    painter.scale(1,-1);

    drawCoordinate(painter);
    drawBackColorBySlc(painter);
    drawCoorScale(painter);
    drawCurveByColor(painter);
    drawAllPoint(painter);

    if(dragPoint < 0){
        setMouseTracking(true);
        emit dragPointChecked(false);
    }
    painter.end();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int x = (event->x() - ui->widget->width())*258/ui->table->width();
    int y = (height() - event->y()-ui->widget_2->height())*258/ui->table->height();
    //qDebug() << x << y;
    if(event->button() == Qt::LeftButton){
        newPoint = false;
        dragPoint = -1;
        clickLine = false;
        for(int i = 0; i < linePoint.size();i++){
            int px = linePoint.at(i).x();
            int py = linePoint.at(i).y();
            if(qAbs(x-px) <= 6 && qAbs(y-py) <= 6)
            {
                newPoint = true;
                for(int j = 0; j < pointLst.size(); j++){
                    int lx = pointLst.at(j).x();
                    int ly = pointLst.at(j).y();
                    if(qAbs(x-lx) <= 6 && qAbs(y-ly) <= 6){
                        dragPoint = j;
                        clickLine = true;
                        leftMouseMv = true;
                        newPoint = false;
                        emit dragPointChecked(true);
                        setMouseTracking(false);
                        update();
                        break;
                    }
                }
                if(newPoint && pointLst.size() < 4 && x > pointLst.at(0).x() && x < pointLst.at(1).x()){
                    pointLst.append(QPoint(x,y));
                    dragPoint = pointLst.size()-1;
                    leftMouseMv = true;
                    clickLine = true;
                    emit dragPointChecked(true);
                    setMouseTracking(false);
                    update();
                }
            }
        }
    }
    else if(event->button() == Qt::RightButton){
        for(int j = 0; j < pointLst.size(); j++){
            int lx = pointLst.at(j).x();
            int ly = pointLst.at(j).y();
            if(qAbs(x-lx) <= 7 && qAbs(y-ly) <= 5){
                if(j > 1)
                    pointLst.removeAt(j);
                if(j == dragPoint){
                    dragPoint = -1;
                    emit dragPointChecked(false);
                }else if(j > dragPoint){
                    dragPoint = dragPoint;
                }else {
                    dragPoint = dragPoint-1;
                }
                update();
                break;
            }
        }
    }
    if(!clickLine){
        dragPoint = -1;
        emit dragPointChecked(false);
        setMouseTracking(true);
        update();
    }
    emit mouseCoordSig(QPoint(x,y));
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int x = (event->x() - ui->widget->width())*258/ui->table->width();
    int y = (height() - event->y()-ui->widget_2->height())*258/ui->table->height();
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x >= 255) x = 255;
    if(y >= 255) y = 255;
    //qDebug() << x << y;
    if(leftMouseMv && dragPoint >= 0)
    {
        if(pointLst[dragPoint] != pointLst.at(0) && pointLst[dragPoint] != pointLst.at(1)){
            if(x <= pointLst.at(0).x()) x = pointLst.at(0).x() + 1;
            if(x >= pointLst.at(1).x()) x = pointLst.at(1).x() - 1;

            if(pointLst.size() == 4){
                if(dragPoint == 2){
                    if(pointLst[2].x() > pointLst[3].x() && x <= pointLst[3].x())
                        x = pointLst[3].x() + 1;
                    if(pointLst[2].x() < pointLst[3].x() && x >= pointLst[3].x())
                        x = pointLst[3].x() - 1;
                }
                else if(dragPoint == 3){
                    if(pointLst[2].x() < pointLst[3].x() && x <= pointLst[2].x())
                        x = pointLst[2].x() + 1;
                    if(pointLst[2].x() > pointLst[3].x() && x >= pointLst[2].x())
                        x = pointLst[2].x() - 1;
                }
            }
        }
        else if(pointLst[dragPoint] == pointLst.at(0)){
            int min = 999;
            for(int i = 1; i < pointLst.size(); i++){
                if(min > pointLst[i].x()) min = pointLst[i].x();
            }
            if(x >= min) x = min-1;
        }
        else if(pointLst[dragPoint] == pointLst.at(1)){
            if(pointLst.size() > 2){
                int max = 0;
                for(int i = 2; i < pointLst.size(); i++){
                    if(max < pointLst[i].x()) max = pointLst[i].x();
                }
                if(x <= max) x = max+1;
            }
            else if(pointLst.size() <= 2)
                if(x <= pointLst.at(0).x()) x = pointLst.at(0).x()+1;
        }
        pointLst[dragPoint].setX(x);
        pointLst[dragPoint].setY(y);
        emit lineChangeSig();
        update();
    }
    emit mouseCoordSig(QPoint(x,y));
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    leftMouseMv = false;
}

void Widget::enterEvent(QEvent *)
{
    setFocus(Qt::MouseFocusReason);
}

void Widget::leaveEvent(QEvent *)
{
    if(dragPoint < 0)
        emit mouseLeaveSig();
}

void Widget::setHist_RGB(const QVector<int> &hist)
{
    rgbBackColor = QVector<int>(hist);
}

QVector<int> Widget::getHist_RED() const
{
    return redBackColor;
}

void Widget::setHist_RED(const QVector<int> &hist)
{
    redBackColor = QVector<int>(hist);
}

QVector<int> Widget::getHist_RGB() const
{
    return rgbBackColor;
}

void Widget::setHist_GREEN(const QVector<int> &hist)
{
    greenBackColor = QVector<int>(hist);
}

QVector<int> Widget::getHist_GREEN() const
{
    return greenBackColor;
}

void Widget::setHist_BLUE(const QVector<int> &hist)
{
    blueBackColor = QVector<int>(hist);
}

QVector<int> Widget::getHist_BLUE() const
{
    return blueBackColor;
}

void Widget::setHist_GRAY(const QVector<int> &hist)
{
    grayBackColor = QVector<int>(hist);
}

QVector<int> Widget::getHist_GRAY() const
{
    return grayBackColor;
}

void Widget::drawCoordinate(QPainter &painter)
{
    painter.setBrush(QColor(79,79,79));
    painter.drawRect(0, 0, 260, 260);
    painter.drawLine(-2000,0,2000,0);
    painter.drawLine(0,1500,0,-1500);
}

void Widget::drawBackgroudColor(QPainter &painter,QVector<int> &curCol)
{
    painter.setPen(QPen(color,1));
    painter.setBrush(color);
    int max = 0;
    for(int i = 0; i < curCol.size(); i++)
    {
        if(curCol.at(i) > max)
        {
            max = curCol.at(i);
        }
    }
    for(int i = 0; i < curCol.size(); i++)
    {
        painter.drawRect(i, 0, 1, curCol.at(i)*255/max);
    }

}

void Widget::drawBackColorBySlc(QPainter &painter)
{
    switch(selectCol){
    case RGB:
        drawBackgroudColor(painter,rgbBackColor);
        break;
    case GREEN:
        drawBackgroudColor(painter,greenBackColor);
        break;
    case BLUE:
        drawBackgroudColor(painter,blueBackColor);
        break;
    case RED:
        drawBackgroudColor(painter,redBackColor);
        break;
    case GRAY:
        drawBackgroudColor(painter,grayBackColor);
        break;
    }
}

void Widget::drawCoorScale(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray,1));
    for(int i = 0; i < 260 ; i+=50)
    {
        if(i%50 == 0 && i >=50)
        {
            QVector<qreal> dashes;
            qreal space = 3;
            dashes << 5 << space << 5 <<space;
            QPen pen;
            pen.setDashPattern(dashes);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(i,300,i,0);
            painter.drawLine(0,i,300,i);
        }
    }
    QLine line(0,0,255,255);
    painter.drawLine(line);
}

void Widget::drawLineFromPoint(QList<QPoint> &plst, const int& col)
{
    double **a;
    int len = plst.size();
    a = new double *[len];
    for(int i = 0; i < len; i++){
        a[i] = new double[len+1];
    }
    /*******通过pointLst的所有点给二维数组赋值生成行列式*******/
    for(int i = 0; i < len; i++){
        for(int j = 0;j < len + 1; j++){
            if(j < len)
                a[i][j] = caculateAllMi(plst.at(i).x(),len-j-1);
            if(j == len)
                a[i][j] = plst.at(i).y();
        }
    }

    /*******调用高斯消元法计算曲线方程的系数值********/
    GaoSiXY gaoSi;
    double *result = new double[len] ;
    gaoSi.solve(a,len,result);

    /********保存曲线方程 x 的系数(a,b,c,d....)********/
    QList<double> xiNum;
    for(int i = 0; i < len; i++){
        xiNum.append(*(result+i));
    }

    linePoint.clear();
    linePoints.clear();
    for(int i = 0; i < 256; i++){
        int x = i;
        double yVal = 0;
        for(int j = 0; j < len; j++){
            yVal += xiNum.at(j)*caculateAllMi(x,len-j-1);
        }
        int y = (int)yVal;
        if(y > 255) y = 255;
        if(y < 0) y = 0;
        if(x < plst.at(0).x()) y = plst.at(0).y();
        if(x > plst.at(1).x()) y = plst.at(1).y();
        allLinePoints[x] = y;
        linePoints.append(y);
        linePoint.append(QPoint(x,y));
    }


    if(col == RED){
        redALLPoint.clear();
        for(int i = 0; i < linePoint.size(); i++)
            redALLPoint.append(linePoint.at(i).y());
    }
    if(col == GREEN){
        greenALLPoint.clear();
        for(int i = 0; i < linePoint.size(); i++)
            greenALLPoint.append(linePoint.at(i).y());
    }
    if(col == BLUE){
        blueALLPoint.clear();
        for(int i = 0; i < linePoint.size(); i++)
            blueALLPoint.append(linePoint.at(i).y());
    }
    if(col == GRAY){
        grayALLPoint.clear();
        for(int i = 0; i < linePoint.size(); i++)
            grayALLPoint.append(linePoint.at(i).y());
    }
    if(col == RGB){
        rgbALLPoint.clear();
        for(int i = 0; i < linePoint.size(); i++)
        {
            rgbALLPoint.append(linePoint.at(i).y());
        }
    }


    for(int i = 0;i < len; i++){
        delete[] a[i];
    }
    delete []a;
}

void Widget::drawAllPoint(QPainter &painter)
{
    if(pointLst.size() > 0)
    {
        painter.setPen(QPen(Qt::white,1));
        painter.setBrush(Qt::white);
        for(int i = 0; i < pointLst.size(); i++){
            painter.drawEllipse(pointLst.at(i),3,3);
        }
        if(dragPoint >= 0){
            painter.setPen(QPen(QColor(0, 245, 255),2));
            painter.drawEllipse(pointLst[dragPoint],4,4);
        }
    }

    if(selectCol == GREEN){
        greenLine.clear();
        greenLine = QList<QPoint>(pointLst);
    }
    else if(selectCol == RED){
        redLine.clear();
        redLine = QList<QPoint>(pointLst);
    }else if(selectCol == BLUE){
        blueLine.clear();
        blueLine = QList<QPoint>(pointLst);
    }else if(selectCol == RGB){
        rgbLine.clear();
        rgbLine = QList<QPoint>(pointLst);
    }else if(selectCol == GRAY){
        grayLine.clear();
        grayLine = QList<QPoint>(pointLst);
    }
}

void Widget::drawCurveByColor(QPainter &painter)
{
    qDebug() << selectCol;
    switch (selectCol) {
    case RGB:
        painter.setPen(QPen(Qt::white,1));
        drawLineFromPoint(rgbLine, RGB);
        drawLineByVector(painter,rgbALLPoint);
        if(redLine.size()>2 || redLine.at(0)!= QPoint(0,0) || redLine.at(1)!= QPoint(255,255)){
            painter.setPen(QPen(Qt::red,1));
            drawLineFromPoint(redLine,RED);
            drawLineByVector(painter,redALLPoint);
        }
        if(blueLine.size()>2 || blueLine.at(0)!= QPoint(0,0) || blueLine.at(1)!= QPoint(255,255)){
            painter.setPen(QPen(Qt::blue,1));
            drawLineFromPoint(blueLine, BLUE);
            drawLineByVector(painter,blueALLPoint);
        }
        if(greenLine.size()>2 || greenLine.at(0)!= QPoint(0,0) || greenLine.at(1)!= QPoint(255,255)){
            painter.setPen(QPen(Qt::green,1));
            drawLineFromPoint(greenLine, BLUE);
            drawLineByVector(painter,greenALLPoint);
        }
        break;
    case RED:
        painter.setPen(QPen(Qt::red,1));
        drawLineFromPoint(redLine,RED);
        drawLineByVector(painter,redALLPoint);
        break;
    case BLUE:
        painter.setPen(QPen(Qt::blue,1));
        drawLineFromPoint(blueLine, BLUE);
        drawLineByVector(painter,blueALLPoint);
        break;
    case GREEN:
        painter.setPen(QPen(Qt::green,1));
        drawLineFromPoint(greenLine, GREEN);
        drawLineByVector(painter,greenALLPoint);
        break;
    case GRAY:
        painter.setPen(QPen(Qt::white,1));
        drawLineFromPoint(grayLine, GRAY);
        drawLineByVector(painter,grayALLPoint);
        break;
    default:
        break;
    }
    drawLineFromPoint(pointLst, -1);
}

void Widget::drawLineByVector(QPainter &painter,QVector<int> &pVec)
{
    for(int i = 0; i < pVec.size()-1; i++){
        painter.drawLine(QPoint(i,pVec.at(i)),QPoint(i+1,pVec.at(i+1)));
    }
    update();
}

int Widget::caculateAllMi(int num, int n)
{
    int val = 1;
    if(n == 0) return 1;
    for(int i = 0; i < n; i++){
        val *= num;
    }
    return val;
}

void Widget::getCurLineLUT(uchar *table, size_t length)
{
    for(int i = 0; i < length; i++)
    {
        table[i] = allLinePoints[i];
    }
}

QVector<unsigned char> Widget::getCurLinePntVec()
{
    return linePoints;
}

void Widget::updateCurLinePnt(const int& colType)
{
    dragPoint = -1;
    selectCol = colType;
    if(selectCol == GREEN){
        color = QColor(202, 255, 112);
        pointLst.clear();
        pointLst = QList<QPoint>(greenLine);
        ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                    "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                    "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(0, 255, 0, 255))}");
        ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                                  "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                                  "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(0, 255, 0, 255))}");
    }else if(selectCol == BLUE){
        color = QColor(131, 111, 255);
        pointLst.clear();
        pointLst = QList<QPoint>(blueLine);
        ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                    "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                    "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 255, 255))}");
        ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                                  "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                                  "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 255, 255))}");
    }else if(selectCol == RED){
        color = QColor(255, 160, 122);
        pointLst.clear();
        pointLst = QList<QPoint>(redLine);
        ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                    "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                    "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))}");
        ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                                  "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                                  "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))}");
    }else if(selectCol == GRAY){
        color = Qt::gray;
        pointLst.clear();
        pointLst = QList<QPoint>(grayLine);
        ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                    "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                    "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
        ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                                  "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                                  "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
    }else if(selectCol == RGB){
        color = Qt::gray;
        pointLst.clear();
        pointLst = QList<QPoint>(rgbLine);
        ui->widget_2->setStyleSheet("QWidget{background-color:qlineargradient("
                                    "spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                    "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
        ui->widget->setStyleSheet("QWidget{background-color:qlineargradient("
                                  "spread:pad, x1:0, y1:1, x2:0, y2:0, "
                                  "stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))}");
    }
    update();
}

void Widget::setXCoorVal(const int &xVal)
{
    if(dragPoint >= 0){
        int x = xVal;
        if(pointLst[dragPoint] != pointLst.at(0) && pointLst[dragPoint] != pointLst.at(1)){
            if(pointLst.size() == 3 && x > pointLst.at(0).x() && x < pointLst.at(1).x())
                pointLst[dragPoint].setX(x);

            if(pointLst.size() == 4){
                if(dragPoint == 2){
                    if(pointLst[2].x() > pointLst[3].x() && x > pointLst[3].x() && x < pointLst[1].x())
                        pointLst[dragPoint].setX(x);
                    if(pointLst[2].x() < pointLst[3].x() && x < pointLst[3].x() && x > pointLst[0].x())
                        pointLst[dragPoint].setX(x);
                }
                else if(dragPoint == 3){
                    if(pointLst[2].x() < pointLst[3].x() && x > pointLst[2].x() && x < pointLst[1].x())
                        pointLst[dragPoint].setX(x);
                    if(pointLst[2].x() > pointLst[3].x() && x < pointLst[2].x() && x > pointLst[0].x())
                        pointLst[dragPoint].setX(x);
                }
            }
        }
        else if(pointLst[dragPoint] == pointLst.at(0)){
            int min = 999;
            for(int i = 1; i < pointLst.size(); i++){
                if(min > pointLst[i].x()) min = pointLst[i].x();
            }
            if(x < min) pointLst[dragPoint].setX(x);
        }
        else if(pointLst[dragPoint] == pointLst.at(1)){
            if(pointLst.size() > 2){
                int max = 0;
                for(int i = 2; i < pointLst.size(); i++){
                    if(max < pointLst[i].x()) max = pointLst[i].x();
                }
                if(x > max) pointLst[dragPoint].setX(x);
            }
            else if(pointLst.size() <= 2)
                if(x > pointLst.at(0).x()) pointLst[dragPoint].setX(x);
        }
        update();
    }
}

void Widget::setYCoorVal(const int &yVal)
{
    if(dragPoint >= 0){
        pointLst[dragPoint].setY(yVal);
        update();
    }
}

void Widget::initSelectColLine(const int& colType)
{
    pointLst.clear();
    pointLst.append(QPoint(0,0));
    pointLst.append(QPoint(255,255));
    switch(colType){
    case RED:
        redLine.clear();
        redLine.append(QPoint(0,0));
        redLine.append(QPoint(255,255));
        break;
    case BLUE:
        blueLine.clear();
        blueLine.append(QPoint(0,0));
        blueLine.append(QPoint(255,255));
        break;
    case GREEN:
        greenLine.clear();
        greenLine.append(QPoint(0,0));
        greenLine.append(QPoint(255,255));
        break;
    case GRAY:
        grayLine.clear();
        grayLine.append(QPoint(0,0));
        grayLine.append(QPoint(255,255));
        break;
    case RGB:
        initAllLstPnt();
        break;
    }
    dragPoint = -1;
    leftMouseMv = false;
    newPoint = false;
    clickLine = false;
    setMouseTracking(true);
    qDebug() << "init";
    updateCurLinePnt(colType);
}

void Widget::SetAllLinePnt(QVector<int> &all)
{
    allLinePntSet = QVector<int>(all);
}

