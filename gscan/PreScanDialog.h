#ifndef PRESCANDIALOG_H
#define PRESCANDIALOG_H

#include <QDialog>
#include "mygraphicsscene.h"

namespace Ui {
class PreScanDialog;
}

class PreScanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreScanDialog(QWidget *parent = nullptr);
    ~PreScanDialog();
    void setImages(QString img_front,QString img_back);

private:
    Ui::PreScanDialog *ui;
    QImage frontImg,backImg;
    MyGraphicsScene frontScene,backScene;
};

#endif // PRESCANDIALOG_H
