#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QCursor>

namespace Ui {
class cutDialog;
}

enum SIZETYPE{
    MILLIM ,
    INCH,
    PIXEL
};

class  cutDialog : public QWidget
{
    Q_OBJECT

public:
    explicit cutDialog(QWidget *parent = nullptr);
    ~cutDialog();
    void setPaperSize(QString type = "A1", const int w = 200);
    void setDpiValue(const double d);
    void setSizeType(const SIZETYPE& t);
    void setCutRectWidth(double w);
    void setCutRectHeight(double h);
    void setCutRectStartX(double x);
    void setCutRectStartY(double y);
    QSizeF getPaperSize()const;
    QSizeF getCutRectSize()const;
    QPointF getCutRectStartPos()const;
    QRectF getCutRectPixel()const;
    void setCutRectPixel(QRectF& rect);
    int getCutRectRight()const;
    int getCutRectBottom()const;
    void refreshView();

signals:
    void cutRectX(double x);
    void cutRectY(double y);
    void cutRectWidth(double w);
    void cutRectHeight(double h);
    void lineEditEnable(bool b);

private:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    //void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
    void drawScale(QPainter &painter);
    void drawCutRect(QPainter &painter);
    void drawTransparentColor(QPainter &painter);
    void drawPaperSize();
    void mousePosition(const QPoint &e);
    void initAllCorner();
    //void mouseChangeRect(int dx,int dy);

private:
    Ui::cutDialog *ui;
    QPointF m_startPoint;
    QPointF m_endPoint;
    QPointF m_moveStart;

    QRectF m_rect;
    QRectF scaleRec;

    QString paperType;
    double paperWidth;
    double paperHeight;
    int divisor;
    int sizeType;
    double dpi;
    double h_w;
    double realRate;
    double sizeRate;

    bool m_mouse_down;
    bool m_left;
    bool m_right;
    bool m_bottom;
    bool m_top;
    bool m_leftCorn;
    bool m_rightCorn;
    bool m_topCorn;
    bool m_bottomCorn;
    bool m_leftTop;
    bool m_rightTop;
    bool m_leftBottom;
    bool m_rightBottom;
    bool m_out;
};

#endif // CUTDIALOG_H
