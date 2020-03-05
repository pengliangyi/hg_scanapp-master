#ifndef CUTPAPERTOOL_H
#define CUTPAPERTOOL_H

#include "cutdialogdef.h"
#include <QDialog>
#include "cutdialog.h"

namespace Ui {
class CutPaperTool;
}

class GCUTDIALOG_SHARED_EXPORT CutPaperTool : public QDialog
{
    Q_OBJECT

public:
    explicit CutPaperTool(QWidget *parent = nullptr);
    ~CutPaperTool();
    void setPaperType(const int dpi, const QString& t,const int& w=200);
    QRectF getCutRectPixel();
    void setCutRect(QRectF& rect);
    void setSizeInit();


private:
    void paintEvent(QPaintEvent *);
    void setSizeLabel();

private slots:
    void cutRectXSlot(double x);
    void cutRectYSlot(double y);
    void cutRectWidthSlot(double w);
    void cutRectHeightSlot(double h);
    void lineEditEnableSlot(bool b);

    void on_startXEdt_textEdited(QString arg1);

    void on_startYEdt_textEdited(QString arg1);

    void on_rectWidth_textEdited(QString arg1);

    void on_rectHeight_textEdited(QString arg1);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pbtn_init_clicked();

private:
    Ui::CutPaperTool *ui;
    SIZETYPE type;
    int dpi;
    QString paperType;
};

#endif // CUTPAPERTOOL_H
