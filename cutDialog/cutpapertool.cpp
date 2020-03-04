#include "cutpapertool.h"
#include "ui_cutpapertool.h"
#include <QDebug>
#include <QValidator>

CutPaperTool::CutPaperTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutPaperTool)
{
    ui->setupUi(this);
    connect(ui->widget,SIGNAL(cutRectX(double)),this,SLOT(cutRectXSlot(double)));
    connect(ui->widget,SIGNAL(cutRectY(double)),this,SLOT(cutRectYSlot(double)));
    connect(ui->widget,SIGNAL(cutRectWidth(double)),this,SLOT(cutRectWidthSlot(double)));
    connect(ui->widget,SIGNAL(cutRectHeight(double)),this,SLOT(cutRectHeightSlot(double)));
    connect(ui->widget,SIGNAL(lineEditEnable(bool)),this,SLOT(lineEditEnableSlot(bool)));
    //setDpi(200);
    //setPaperType(200,"A4",400);
    ui->widget->setSizeType(MILLIM);
    setSizeLabel();
    this->setFixedWidth(ui->widget->width()+20);
}

CutPaperTool::~CutPaperTool()
{
    delete ui;
}

void CutPaperTool::paintEvent(QPaintEvent *)
{

}

void CutPaperTool::setPaperType(const int d, const QString &t, const int& w)
{
    dpi = d;
    ui->dpiLab->setText(QString::number(dpi));
    ui->widget->setDpiValue(dpi);
    paperType = t;
    ui->paperLab->setText(paperType);
    ui->widget->setPaperSize(paperType,w);
    //ui->widget->refreshView();
    setSizeLabel();
    this->setFixedWidth(ui->widget->width()+20);
    setSizeInit();
}

QRectF CutPaperTool::getCutRectPixel()
{
    return QRectF(ui->widget->getCutRectPixel());
}

void CutPaperTool::setCutRect(QRectF &rect)
{
    ui->widget->setCutRectPixel(rect);

    ui->startXEdt->setText(QString::number(rect.x()/dpi/0.03937));
    ui->startXEdt->setText(QString::number(rect.y()/dpi/0.03937));
    ui->rectWidth->setText(QString::number(rect.width()/dpi/0.03937));
    ui->rectHeight->setText(QString::number(rect.height()/dpi/0.03937));
}

void CutPaperTool::setSizeLabel()
{
    QString wSize;
    QString hSize;
    if(ui->comboBox_2->currentIndex() == INCH){
        wSize = QString::number(ui->widget->getPaperSize().width(),'f',2);
        hSize = QString::number(ui->widget->getPaperSize().height(),'f',2);
    }
    else {
        wSize = QString::number(int(ui->widget->getPaperSize().width()+0.001));
        hSize = QString::number(int(ui->widget->getPaperSize().height()+0.001));
    }
    ui->sizeLabel->setText("("+wSize + " * " +hSize+")");
}

void CutPaperTool::setSizeInit()
{
    ui->widget->setCutRectStartX(0);
    ui->widget->setCutRectStartY(0);
    ui->widget->setCutRectWidth(ui->widget->getPaperSize().width());
    ui->widget->setCutRectHeight(ui->widget->getPaperSize().height());
    ui->startXEdt->setText("0");
    ui->startYEdt->setText("0");
    ui->rectWidth->setText(QString::number(int(ui->widget->getPaperSize().width())));
    ui->rectHeight->setText(QString::number(int(ui->widget->getPaperSize().height())));
}

void CutPaperTool::cutRectXSlot(double x)
{
    ui->startXEdt->setText(QString::number(x));
}

void CutPaperTool::cutRectYSlot(double y)
{
    ui->startYEdt->setText(QString::number(y));
}

void CutPaperTool::cutRectWidthSlot(double w)
{
    ui->rectWidth->setText(QString::number(w));
}

void CutPaperTool::cutRectHeightSlot(double h)
{
    ui->rectHeight->setText(QString::number(h));
}

void CutPaperTool::lineEditEnableSlot(bool b)
{
    ui->startXEdt->setEnabled(b);
    ui->startYEdt->setEnabled(b);
    ui->rectWidth->setEnabled(b);
    ui->rectHeight->setEnabled(b);
}

void CutPaperTool::on_startXEdt_textEdited(QString arg1)
{
    double x  = arg1.toDouble();
    if(x >ui->widget->getCutRectRight()) {
        arg1.chop(1);
        x = arg1.toDouble();
    }
    if(type == PIXEL) x = arg1.toInt();
    ui->widget->setCutRectStartX(x);
}

void CutPaperTool::on_startYEdt_textEdited(QString arg1)
{
    double y  = arg1.toDouble();
    if(y >ui->widget->getCutRectBottom()) {
        arg1.chop(1);
        y = arg1.toDouble();
    }
    if(type == PIXEL) y = arg1.toInt();
    ui->widget->setCutRectStartY(y);
}

void CutPaperTool::on_rectWidth_textEdited(QString arg1)
{
    double v  = arg1.toDouble();
    if(v > ui->widget->getPaperSize().width() - ui->widget->getCutRectStartPos().x()) {
        arg1.chop(1);
        v = arg1.toDouble();
    }
    if(type == PIXEL) v = arg1.toInt();
    ui->widget->setCutRectWidth(v);
}

void CutPaperTool::on_rectHeight_textEdited(QString arg1)
{
    double v  = arg1.toDouble();
    if(v > ui->widget->getPaperSize().height() - ui->widget->getCutRectStartPos().y()) {
        arg1.chop(1);
        v = arg1.toDouble();
    }
    if(type == PIXEL) v = arg1.toInt();
    ui->widget->setCutRectHeight(v);
}

void CutPaperTool::on_comboBox_2_currentIndexChanged(int index)
{
    switch(index){
    case 0:
        ui->xLabel->setText("mm");
        ui->yLabel->setText("mm");
        ui->wLabel->setText("mm");
        ui->hLabel->setText("mm");
        ui->widget->setSizeType(MILLIM);
        break;
    case 1:
        ui->xLabel->setText("in");
        ui->yLabel->setText("in");
        ui->wLabel->setText("in");
        ui->hLabel->setText("in");
        ui->widget->setSizeType(INCH);
        break;
    case 2:
        ui->xLabel->setText("px");
        ui->yLabel->setText("px");
        ui->wLabel->setText("px");
        ui->hLabel->setText("px");
        ui->widget->setSizeType(PIXEL);
        break;
    }
    update();
    setSizeLabel();
    ui->widget->refreshView();
}


void CutPaperTool::on_buttonBox_accepted()
{
    close();
}

void CutPaperTool::on_buttonBox_rejected()
{
    close();
}

void CutPaperTool::on_pbtn_init_clicked()
{
    setSizeInit();
}
