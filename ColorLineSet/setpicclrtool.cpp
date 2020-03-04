#include "setpicclrtool.h"
#include "ui_setpicclrtool.h"
#include "widget.h"
#include <QDebug>
#include <QVariant>

setPicClrTool::setPicClrTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPicClrTool)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->inputEdt->setEnabled(false);
    ui->outputEdt->setEnabled(false);
    ui->inputEdt->setValidator(new QIntValidator(0, 255, this));
    ui->outputEdt->setValidator(new QIntValidator(0, 255, this));
    ui->widget->setFocus(Qt::MouseFocusReason);
    connect(ui->widget,SIGNAL(mouseCoordSig(QPoint)),this,SLOT(mouseCoordSlot(QPoint)));
    connect(ui->widget,SIGNAL(dragPointChecked(bool)),this,SLOT(lineEditEnable(bool)));
    connect(ui->widget,SIGNAL(lineChangeSig()),this,SLOT(lineChangeSlot()));
}

setPicClrTool::~setPicClrTool()
{
    delete ui;
}

void setPicClrTool::getGrayTable(uchar *table, size_t length)
{
    QVector<int> gray = getGrayALLPoint();
    for (size_t i = 0; i < length; i++)
            table[i] = static_cast<uchar>(gray[static_cast<int>(i)]);
}

void setPicClrTool::getRGBTable(uchar *table, size_t length)
{
    if (length != 768)
            throw "length is not 768.";

    QVector<int> rgb = getRgbALLPoint();
    QVector<int> red = getRedALLPoint();
    QVector<int> green = getGreenALLPoint();
    QVector<int> blue = getBlueALLPoint();


    for (size_t i = 0; i < 256; i++)
    {
        if (i == 255)
            int a = 0;
        //table member order is B\G\R
        table[i * 3 + 0] = static_cast<uchar>(blue[rgb[static_cast<int>(i)]]);
        table[i * 3 + 1] = static_cast<uchar>(green[rgb[static_cast<int>(i)]]);
        table[i * 3 + 2] = static_cast<uchar>(red[rgb[static_cast<int>(i)]]);
    }
}

//void setPicClrTool::setGrayTable(const uchar *table, size_t length)
//{
//    QVector<int> gray;
//    for (size_t i = 0; i < length; i++)
//            gray[static_cast<int>(i)] = int(table[i]);
//    //ui->widget->setGrayALLPoint(gray);
//}

//void setPicClrTool::setRGBTable(const uchar *table, size_t length)
//{
//    QVector<int> rgb;
//    QVector<int> red;
//    QVector<int> green;
//    QVector<int> blue;
//    for(int i = 0; i < 256; i++){
//        rgb.append(i);
//    }
//    for (size_t i = 0; i < length; i++){
//            red.append(int(table[i * 3 + 0]));
//            green.append(int(table[i * 3 + 1]));
//            blue.append(int(table[i * 3 + 2]));
//    }

//}

void setPicClrTool::setColorMode(int colorMode)
{
    QVariant v(6);
    if(colorMode == 0)
    {
        ui->colorSetCmb->setItemData(1,v,Qt::UserRole - 1);
    }
    else
    {
        ui->comboBox->setItemData(2,v,Qt::UserRole - 1);
        ui->colorSetCmb->setItemData(0,v,Qt::UserRole - 1);
        ui->colorSetCmb->setItemData(2,v,Qt::UserRole - 1);
        ui->colorSetCmb->setItemData(3,v,Qt::UserRole - 1);
        ui->colorSetCmb->setItemData(4,v,Qt::UserRole - 1);
        ui->colorSetCmb->setCurrentIndex(1);
    }
}

QVector<QList<QPoint> > setPicClrTool::getRGBKeyTable()
{
    QVector<QList<QPoint>> plv;
    plv.append(getRgbKeyPoint());
    plv.append(getRedKeyPoint());
    plv.append(getGreenKeyPoint());
    plv.append(getBlueKeyPoint());
    return plv;
}

void setPicClrTool::setRGBKeyTable(QVector<QList<QPoint> > &plv)
{
    setRgbKeyPoint(plv[0]);
    setRedKeyPoint(plv[1]);
    setGreenKeyPoint(plv[2]);
    setBlueKeyPoint(plv[3]);
}

QList<QPoint> setPicClrTool::getGrayKeyTable()
{
    return getGrayKeyPoint();
}

void setPicClrTool::setGrayKeyTable(QList<QPoint> &plv)
{
    setGrayKeyPoint(plv);
}

QVector<int> setPicClrTool::getRgbALLPoint()
{
    return ui->widget->getRgbALLPoint();
}

QList<QPoint> setPicClrTool::getRgbKeyPoint() const
{
    qDebug() << "setRgb" << ui->widget->getRgbKeyPoint();
    return ui->widget->getRgbKeyPoint();
}

void setPicClrTool::setRgbKeyPoint(const QList<QPoint> &pVec)
{
    qDebug() << "setRgb" << pVec;
    ui->widget->setRgbKeyPoint(pVec);
}

QList<QPoint> setPicClrTool::getRedKeyPoint() const
{
    return ui->widget->getRedKeyPoint();
}

void setPicClrTool::setRedKeyPoint(const QList<QPoint> &pVec)
{
    ui->widget->setRedKeyPoint(pVec);
}

QList<QPoint> setPicClrTool::getBlueKeyPoint() const
{
    return ui->widget->getBlueKeyPoint();
}

void setPicClrTool::setBlueKeyPoint(const QList<QPoint> &pVec)
{
    ui->widget->setBlueKeyPoint(pVec);
}

QList<QPoint> setPicClrTool::getGreenKeyPoint() const
{
    return ui->widget->getGreenKeyPoint();
}

void setPicClrTool::setGreenKeyPoint(const QList<QPoint> &pVec)
{
    ui->widget->setGreenKeyPoint(pVec);
}

QList<QPoint> setPicClrTool::getGrayKeyPoint() const
{
    return ui->widget->getGrayKeyPoint();
}

void setPicClrTool::setGrayKeyPoint(const QList<QPoint> &pVec)
{
    ui->widget->setGrayKeyPoint(pVec);
}

QVector<int> setPicClrTool::getRedALLPoint()
{
    return ui->widget->getRedALLPoint();
}

QVector<int> setPicClrTool::getBlueALLPoint()
{
    return ui->widget->getBlueALLPoint();
}

QVector<int> setPicClrTool::getGreenALLPoint()
{
    return ui->widget->getGreenALLPoint();
}

QVector<int> setPicClrTool::getGrayALLPoint()
{
    return ui->widget->getGrayALLPoint();
}

void setPicClrTool::mouseCoordSlot(QPoint pos)
{
    ui->inputEdt->setText(QString::number(pos.x()));
    ui->outputEdt->setText(QString::number(pos.y()));
}

void setPicClrTool::lineEditEnable(bool a)
{
    ui->inputEdt->setEnabled(a);
    ui->outputEdt->setEnabled(a);
}

void setPicClrTool::lineChangeSlot()
{
    ui->comboBox->setCurrentIndex(0);
}

void setPicClrTool::on_colorSetCmb_currentIndexChanged(int index)
{
    switch(index){
    case RED:
        ui->widget->updateCurLinePnt(RED);
        break;
    case GREEN:
        ui->widget->updateCurLinePnt(GREEN);
        break;
    case BLUE:
        ui->widget->updateCurLinePnt(BLUE);
        break;
    case RGB:
        ui->widget->updateCurLinePnt(RGB);
        break;
    case GRAY:
        ui->widget->updateCurLinePnt(GRAY);
        break;
    }
}

void setPicClrTool::on_comboBox_currentIndexChanged(int index)
{  
    if(index == 0){
        ui->widget->initAllLstPnt();
        ui->widget->updateCurLinePnt(ui->colorSetCmb->currentIndex());
        return ;
    }
    ui->widget->initAllLstPnt();
    if(index == 1){
        rgbLine.clear();
        rgbLine.append(QPoint(0,255));
        rgbLine.append(QPoint(255,0));
        ui->widget->setLstPnt_RGB(rgbLine);
    }else if(index == 2){
        redLine.clear();
        redLine.append(QPoint(33,255));
        redLine.append(QPoint(185,0));
        redLine.append(QPoint(119,127));
        greenLine.clear();
        greenLine.append(QPoint(28,255));
        greenLine.append(QPoint(132,0));
        greenLine.append(QPoint(77,127));
        blueLine.clear();
        blueLine.append(QPoint(25,255));
        blueLine.append(QPoint(108,0));
        blueLine.append(QPoint(60,127));
        ui->widget->setLstPnt_RED(redLine);
        ui->widget->setLstPnt_GREEN(greenLine);
        ui->widget->setLstPnt_BLUE(blueLine);
    }else if(index == 4){
        rgbLine.clear();
        rgbLine.append(QPoint(0,0));
        rgbLine.append(QPoint(255,255));
        rgbLine.append(QPoint(103,125));
        ui->widget->setLstPnt_RGB(rgbLine);
    }else if(index == 3){
        rgbLine.clear();
        rgbLine.append(QPoint(0,0));
        rgbLine.append(QPoint(255,255));
        rgbLine.append(QPoint(130,101));
        ui->widget->setLstPnt_RGB(rgbLine);
    }
    ui->widget->updateCurLinePnt(ui->colorSetCmb->currentIndex());
}

void setPicClrTool::on_inputEdt_textChanged(const QString &arg1)
{
    ui->widget->setXCoorVal(arg1.toInt());
}

void setPicClrTool::on_outputEdt_textChanged(const QString &arg1)
{
    ui->widget->setYCoorVal(arg1.toInt());
}

void setPicClrTool::on_pushButton_clicked()
{
    ui->widget->initSelectColLine(ui->colorSetCmb->currentIndex());
}

void setPicClrTool::on_buttonBox_accepted()
{
    close();
}

void setPicClrTool::on_buttonBox_rejected()
{
    ui->widget->initAllLstPnt();
    close();
}
