#include "cutdialog.h"
#include "ui_cutdialog.h"
#include <QDebug>
#include <QToolTip>

cutDialog::cutDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cutDialog)
{
    ui->setupUi(this);
    divisor = 8;
    dpi = 1;
    sizeType = 0;
    paperWidth = 185;
    h_w =  260.0/185.0;
    sizeRate = 1;
    paperHeight = paperWidth*h_w;

    this->setFixedSize(paperWidth,paperHeight);
    setMouseTracking(true);
    m_startPoint = QPoint(10,10);
    m_endPoint = QPoint(50,50);
    m_mouse_down = false;

    initAllCorner();
}

cutDialog::~cutDialog()
{
    delete ui;
}

void cutDialog::setPaperSize(QString type, const int w)
{
    paperType = type;

    if(paperType == "A3") h_w = 420.0/297;
    else if(paperType == "A4") h_w = 297.0/210;
    else if(paperType == "A4R") h_w = 210/297.0;
    else if(paperType == "A5") h_w = 210.0/148;
    else if(paperType == "A5R") h_w = 148/210.0;
    else if(paperType == "A6") h_w = 148.0/105;
    else if(paperType == "A6R") h_w = 105/148.0;
    else if(paperType == "B4") h_w = 353.0/250;
    else if(paperType == "B5") h_w = 250.0/176;
    else if(paperType == "B5R") h_w = 176/250.0;
    else if(paperType == "B6") h_w = 176.0/125;
    else if(paperType == "B6R") h_w = 125/176.0;
    else if(paperType == "Letter") h_w = 279.0/210;
    else if(paperType == "LetterR") h_w = 210.0/279;
    else if(paperType == "Double Letter") h_w = 850.0/550;
    else if(paperType == "Legal") h_w = 2800.0/1700;
    else if(paperType == "Max Size") h_w = 3307.0*2/2338;
    else if(paperType == "Auto") {
        if(dpi == 100.0) h_w = 1795.0/1189;
        if(dpi == 150.0) h_w = 1795.0/1784;
        if(dpi == 200.0) h_w = 3307.0/2338;
        if(dpi == 240.0) h_w = 4308.0/2854;
        if(dpi == 200.0) h_w = 5385.0/3567;
    };


    if(type.contains("3")) divisor = 2;
    else if (type.contains("4")) divisor = 4;
    else if (type.contains("5")) divisor = 6;
    else if (type.contains("6")) divisor = 8;
    else divisor = 4;
    paperWidth = w;
    if(type.contains("R")) paperWidth = paperWidth*h_w*1.5;
    double realW = paperWidth;

    if(paperType == "A3" || paperType == "A4R") realRate = 297.0/realW;
    else if(paperType == "A4" || paperType == "A5R") realRate = 210.0/realW;
    else if(paperType == "A5" || paperType == "A6R") realRate = 148.0/realW;
    else if(paperType == "A6") realRate = 105.0/realW;
    else if(paperType == "B4" || paperType == "B5R") realRate = 250.0/realW;
    else if(paperType == "B5" || paperType == "B6R") realRate = 176.0/realW;
    else if(paperType == "B6") realRate = 125.0/realW;
    else if(paperType == "Letter") realRate = 216.0/realW;
    else if(paperType == "LetterR") realRate = 279.0/realW;
    else if(paperType == "Double Letter") realRate = 432.0/realW;
    else if(paperType == "Legal") realRate = 297.0/realW;
    else if(paperType == "Max Size") realRate = 297.0/realW;
    else if(paperType == "Auto") {
        if(dpi == 100.0) realRate = 1189.0*0.039377/dpi/realW;
        if(dpi == 150.0) realRate = 1784*0.039377/dpi/realW;
        if(dpi == 200.0) realRate = 2338*0.039377/dpi/realW;
        if(dpi == 240.0) realRate = 2854*0.039377/dpi/realW;
        if(dpi == 200.0) realRate = 3567*0.039377/dpi/realW;
    };

    paperHeight = paperWidth*h_w;
    this->setFixedSize(paperWidth+4,paperHeight+4);
    scaleRec = QRectF(0,0,paperWidth,paperHeight+0.5);
    m_rect = QRectF(m_startPoint,m_endPoint);
    update();
    refreshView();
}

void cutDialog::setDpiValue(const double d)
{
    dpi = d;
    if(sizeType == PIXEL) sizeRate = 0.03937*dpi;
    update();
}

void cutDialog::setSizeType(const SIZETYPE &t)
{
    sizeType = t;
    switch (sizeType) {
    case MILLIM:
        sizeRate = 1;
        break;
    case INCH:
        sizeRate = 0.03937;
        break;
    case PIXEL:
        sizeRate = 0.03937*dpi;
        break;
    }
    update();
}

void cutDialog::setCutRectWidth(double w)
{
    m_endPoint.setX(m_startPoint.x()+scaleRec.width()*w/getPaperSize().width());
    update();
}

void cutDialog::setCutRectHeight(double h)
{
    m_endPoint.setY(m_startPoint.y()+scaleRec.height()*h/getPaperSize().height());
    update();
}

void cutDialog::setCutRectStartX(double x)
{
    m_startPoint.setX(scaleRec.width()*x/getPaperSize().width());
    update();
}

void cutDialog::setCutRectStartY(double y)
{
    m_startPoint.setY(scaleRec.height()*y/getPaperSize().height());
    update();
}

QSizeF cutDialog::getPaperSize() const
{
    double realW = paperWidth*realRate*sizeRate;
    double realH = paperHeight*realRate*sizeRate;
    return QSizeF(realW,realH);
}

QSizeF cutDialog::getCutRectSize() const
{
    double realCutW = getPaperSize().width()*m_rect.width()/scaleRec.width();
    double realCutH = getPaperSize().height()*m_rect.height()/scaleRec.height();
    return QSizeF(realCutW,realCutH);
}

QPointF cutDialog::getCutRectStartPos() const
{
    double realCutX = getPaperSize().width()*m_startPoint.x()/scaleRec.width();
    double realCutY = getPaperSize().height()*m_startPoint.y()/scaleRec.height();
    return QPointF(realCutX,realCutY);
}

QRectF cutDialog::getCutRectPixel() const
{
    double x = 0.03937*dpi*paperWidth*realRate*m_startPoint.x()/scaleRec.width();
    double y = 0.03937*dpi*paperHeight*realRate*m_startPoint.y()/scaleRec.height();
    double w = 0.03937*dpi*paperWidth*realRate*m_rect.width()/scaleRec.width();
    double h = 0.03937*dpi*paperHeight*realRate*m_rect.height()/scaleRec.height();
    return QRectF(x,y,w,h);
}

void cutDialog::setCutRectPixel(QRectF &rect)
{
    m_startPoint.setX(scaleRec.width()*rect.x()/(paperWidth*realRate*0.03937*dpi));
    m_startPoint.setY(scaleRec.height()*rect.y()/(paperHeight*realRate*0.03937*dpi));
    m_endPoint.setX(m_startPoint.x()+scaleRec.width()*rect.width()/(paperWidth*realRate*0.03937*dpi));
    m_endPoint.setY(m_startPoint.y()+scaleRec.height()*rect.height()/(paperHeight*realRate*0.03937*dpi));
    update();
}

int cutDialog::getCutRectRight() const
{
    return getPaperSize().width()*m_rect.right()/scaleRec.right();
}

int cutDialog::getCutRectBottom() const
{
    return getPaperSize().height()*m_rect.bottom()/scaleRec.bottom();
}

void cutDialog::mousePressEvent(QMouseEvent *event)
{
    int x = event->x()/**paperWidth/width()*/;
    int y = event->y()/**paperHeight/height()*/;
    m_moveStart = QPoint(x,y);
    m_mouse_down = event->button() == Qt::LeftButton;
    update();
}

void cutDialog::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x()/**paperWidth/width()*/;
    int y = event->y()/**paperHeight/height()*/;
    if(m_mouse_down){
        int dx = m_moveStart.x() - x;
        int dy = m_moveStart.y() - y;
        m_moveStart = QPoint(x,y);

        if(m_leftCorn){
            m_startPoint.setX(x);
        }else if(m_rightCorn){
            m_endPoint.setX(x);
        }else if(m_topCorn){
            m_startPoint.setY(y);
        }else if(m_bottomCorn){
            m_endPoint.setY(y);
        }else if(m_leftTop){
            m_startPoint.setX(x);
            m_startPoint.setY(y);
        }else if(m_leftBottom){
            m_startPoint.setX(x);
            m_endPoint.setY(y);
        }else if(m_rightTop){
            m_startPoint.setY(y);
            m_endPoint.setX(x);
        }else if(m_rightBottom){
            m_endPoint.setX(x);
            m_endPoint.setY(y);
        }
        else if(!m_out){
            if(m_startPoint.x() - dx < 0) dx = 0;
            if(m_startPoint.y() - dy < 0) dy = 0;
            if(m_endPoint.x() - dx > scaleRec.right()) dx = 0;
            if(m_endPoint.y() - dy > scaleRec.bottom()) dy = 0;
            m_startPoint.setX(m_rect.left()-dx);
            m_startPoint.setY(m_rect.top()-dy);
            m_endPoint.setX(m_rect.right()-dx);
            m_endPoint.setY(m_rect.bottom()-dy);
        }
        if(m_startPoint.x()<=0) m_startPoint.setX(0);
        if(m_startPoint.y()<=0) m_startPoint.setY(0);
        if(m_endPoint.x()>=scaleRec.width()) m_endPoint.setX(scaleRec.right());
        if(m_endPoint.y()>=scaleRec.height()) m_endPoint.setY(scaleRec.bottom());


        if(qAbs(m_startPoint.x() - m_endPoint.x()) <= 10 || m_startPoint.x() > m_endPoint.x()){
            m_startPoint.setX(m_rect.left());
            m_endPoint.setX(m_rect.right());
        }
        if(qAbs(m_startPoint.y() - m_endPoint.y()) <= 10 || m_startPoint.y() > m_endPoint.y()){
            m_startPoint.setY(m_rect.top());
            m_endPoint.setY(m_rect.bottom());
        }

        update();
    }else{
        mousePosition(QPoint(x,y));
    }
}

void cutDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_mouse_down = false;
    update();
}

void cutDialog::enterEvent(QEvent *)
{
    emit lineEditEnable(false);
}

void cutDialog::leaveEvent(QEvent *)
{
    emit lineEditEnable(true);
}

void cutDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(0,0);

    drawScale(painter);
    drawCutRect(painter);
    drawTransparentColor(painter);
    if(m_mouse_down){
        if(sizeType == INCH || sizeType == MILLIM){
            emit cutRectX(QString::number(getCutRectStartPos().x(),'f',2).toDouble());
            emit cutRectY(QString::number(getCutRectStartPos().y(),'f',2).toDouble());
            emit cutRectWidth(QString::number(getCutRectSize().width(),'f',2).toDouble());
            emit cutRectHeight(QString::number(getCutRectSize().height(),'f',2).toDouble());
        }else{
            emit cutRectX(double(int(getCutRectStartPos().x())));
            emit cutRectY(double(int(getCutRectStartPos().y())));
            emit cutRectWidth(double(int(getCutRectSize().width())));
            emit cutRectHeight(double(int(getCutRectSize().height())));
        }
    }

    painter.end();
}

void cutDialog::drawScale(QPainter& painter)
{
    painter.setPen(QPen(Qt::black,1));
    scaleRec = QRectF(0,0,paperWidth,paperHeight+0.5);
    painter.drawRect(scaleRec);
    for(int i = 6 ; i <= paperWidth-6; i++){
        if(int(paperWidth/2) == i || int(paperWidth/4) == i || int(paperWidth*3/4) == i){
            painter.setPen(QPen(Qt::gray,1));
            painter.drawLine(i,0,i,paperHeight);
            painter.setPen(QPen(Qt::black,1));
        }
        if(i%divisor == 0){
            painter.drawLine(i,paperHeight-4,i,paperHeight);
            painter.drawLine(i,0,i,4);
        }
        if(i%(divisor*5) == 0){
            painter.drawLine(i,paperHeight-8,i,paperHeight);
            painter.drawLine(i,0,i,8);
        }
    }
    for(int i = 6 ; i <= paperHeight-6; i++){
        if(int(paperHeight/2) == i || int(paperHeight/4) == i || int(paperHeight*3/4) == i){
            painter.setPen(QPen(Qt::gray,1));
            painter.drawLine(0,i,paperWidth,i);
            painter.setPen(QPen(Qt::black,1));
        }
        if(i%divisor == 0){
            painter.drawLine(0,i,4,i);
            painter.drawLine(paperWidth,i,paperWidth-4,i);
        }
        if(i%(divisor*5) == 0){
            painter.drawLine(0,i,8,i);
            painter.drawLine(paperWidth,i,paperWidth-8,i);
        }
    }
}

void cutDialog::drawCutRect(QPainter &painter)
{
    painter.setPen(QPen(Qt::green,1));
    m_rect = QRectF(m_startPoint,m_endPoint);
    painter.drawRect(m_rect);
    painter.setPen(QPen(Qt::red,1));
    painter.setBrush(Qt::red);
    painter.drawEllipse(m_rect.left()-2,(m_rect.bottom()-m_rect.top())/2+m_rect.top()-2,4,4);
    painter.drawEllipse(m_rect.left()-2,m_rect.top()-2,4,4);
    painter.drawEllipse(m_rect.left()-2,m_rect.bottom()-2,4,4);
    painter.drawEllipse(m_rect.right()-2,m_rect.top()-2,4,4);
    painter.drawEllipse(m_rect.right()-2,m_rect.bottom()-2,4,4);
    painter.drawEllipse(m_rect.right()-2,(m_rect.bottom()-m_rect.top())/2+m_rect.top()-2,4,4);
    painter.drawEllipse((m_rect.right()-m_rect.left())/2+m_rect.left()-2,m_rect.top()-2,4,4);
    painter.drawEllipse((m_rect.right()-m_rect.left())/2+m_rect.left()-2,m_rect.bottom()-2,4,4);
}

void cutDialog::drawTransparentColor(QPainter &painter)
{
    QPainterPath painterPath;
    QPainterPath p;
    p.addRect(scaleRec);
    painterPath.addRect(m_rect);
    QPainterPath drawPath =p.subtracted(painterPath);
    painter.setOpacity(0.7);
    painter.fillPath(drawPath,QBrush(Qt::gray));
}

void cutDialog::mousePosition(const QPoint& e)
{
    initAllCorner();
    QRectF r = QRectF(m_rect);
    int x = e.x();
    int y = e.y();
    m_left = qAbs(x - r.left())  < 5;
    m_right = qAbs(x - r.right()) < 5;
    m_bottom = qAbs(y - r.bottom()) < 5;
    m_top = qAbs(y - r.top()) < 5;
    m_out = r.left()-x>=5 || x-r.right()>=5 || r.top()-y>=5 || y-r.bottom()>=5;
    bool lorr = m_left | m_right;
    bool torb = m_top | m_bottom;
    if(lorr && torb)
    {
       if((m_left && m_top) || (m_right && m_bottom))
           setCursor(Qt::SizeFDiagCursor);
       else
           setCursor(Qt::SizeBDiagCursor);
    }
    else if(lorr)
       setCursor(Qt::SizeHorCursor);
    else if(torb)
       setCursor(Qt::SizeVerCursor);
    else if(!m_out)
    {
       setCursor(Qt::SizeAllCursor);
       m_bottom = m_left = m_right = m_top = false;
    }else if(m_out){
        setCursor(Qt::ArrowCursor);
        m_bottom = m_left = m_right = m_top = false;
    }

    if(m_left && m_top) m_leftTop = true;
    else if(m_left && m_bottom) m_leftBottom = true;
    else if(m_right && m_top) m_rightTop = true;
    else if(m_right && m_bottom) m_rightBottom = true;
    else if(m_left && !m_top && !m_bottom) m_leftCorn = true;
    else if(m_right && !m_top && !m_bottom) m_rightCorn = true;
    else if(m_top && !m_left && !m_right) m_topCorn = true;
    else if(m_bottom && !m_left && !m_right) m_bottomCorn = true;
    else initAllCorner();
}

void cutDialog::refreshView()
{
    if(sizeType == INCH){
        emit cutRectX(QString::number(getCutRectStartPos().x(),'f',2).toDouble());
        emit cutRectY(QString::number(getCutRectStartPos().y(),'f',2).toDouble());
        emit cutRectWidth(QString::number(getCutRectSize().width(),'f',2).toDouble());
        emit cutRectHeight(QString::number(getCutRectSize().height(),'f',2).toDouble());
    }else{
        emit cutRectX(QString::number(getCutRectStartPos().x(),'f',0).toDouble());
        emit cutRectY(QString::number(getCutRectStartPos().y(),'f',0).toDouble());
        emit cutRectWidth(double(int(getCutRectSize().width())));
        emit cutRectHeight(double(int(getCutRectSize().height())));
    }
}

void cutDialog::initAllCorner()
{
    m_left = false;
    m_right = false;
    m_bottom = false;
    m_top = false;
    m_leftCorn = false;
    m_rightCorn = false;
    m_topCorn = false;
    m_bottomCorn = false;
    m_leftTop = false;
    m_leftBottom = false;
    m_rightTop = false;
    m_rightBottom = false;
}
