#ifndef SETPICCLRTOOL_H
#define SETPICCLRTOOL_H

#include "colorlinesetdef.h"
#include <QDialog>

namespace Ui {
class setPicClrTool;
}

class  GCOLORLINESET_SHARED_EXPORT setPicClrTool : public QDialog
{
    Q_OBJECT

public:
    explicit setPicClrTool(QWidget *parent = nullptr);
    ~setPicClrTool();

    void getGrayTable(uchar* table, size_t length = 256);

    void getRGBTable(uchar* table, size_t length = 768);

//    void setGrayTable(const uchar* table, size_t length = 256);

//    void setRGBTable(const uchar* table, size_t length = 768);

    void setColorMode(int colorMode);

    QVector<QList<QPoint>> getRGBKeyTable();
    void setRGBKeyTable(QVector<QList<QPoint>>& plv);
    QList<QPoint> getGrayKeyTable();
    void setGrayKeyTable(QList<QPoint> &plv);


private:
    QVector<int> getRgbALLPoint();
    QVector<int> getRedALLPoint();
    QVector<int> getBlueALLPoint();
    QVector<int> getGreenALLPoint();
    QVector<int> getGrayALLPoint();

    QList<QPoint> getRgbKeyPoint()const;
    void setRgbKeyPoint(const QList<QPoint>& pVec);
    QList<QPoint> getRedKeyPoint()const;
    void setRedKeyPoint(const QList<QPoint>& pVec);
    QList<QPoint> getBlueKeyPoint()const;
    void setBlueKeyPoint(const QList<QPoint>& pVec);
    QList<QPoint> getGreenKeyPoint()const;
    void setGreenKeyPoint(const QList<QPoint>& pVec);
    QList<QPoint> getGrayKeyPoint()const;
    void setGrayKeyPoint(const QList<QPoint>& pVec);

private slots:
    void mouseCoordSlot(QPoint);//set QLineEdit value by QPoint setting    
    void lineEditEnable(bool);//set 2 LineEdit enable status
    void lineChangeSlot();

    void on_colorSetCmb_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_inputEdt_textChanged(const QString &arg1);

    void on_outputEdt_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::setPicClrTool *ui;
    QList<QPoint> linePoint;
    QList<QPoint> greenLine;
    QList<QPoint> redLine;
    QList<QPoint> blueLine;
    QList<QPoint> rgbLine;
    QList<QPoint> grayLine;
};

#endif // SETPICCLRTOOL_H
