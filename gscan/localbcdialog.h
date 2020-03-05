#ifndef LOCALBCDIALOG_H
#define LOCALBCDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QAbstractButton>

namespace Ui {
class LocalBCDialog;
}

class LocalBCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocalBCDialog(QWidget *parent, QImage* src, QString appDirPath);
    ~LocalBCDialog();

private slots:
    void on_Slider_brightness_sliderMoved(int position);
    void on_Slider_contrast_sliderMoved(int position);

    void on_buttonBox_accepted();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::LocalBCDialog *ui;
    QImage srcImg;//src image copy
    QImage *dstImg;//dst image ptr
    QString m_appDirPath;
    QGraphicsScene *dst_pic;//dst image scene
    const QString tmpPath = "./tmp/bcAdjust.jpg";//save tmp dst image for preview
	const int imgSizeWidth = 297;//set fixed width
    const int imgSizeHeight = 420;//set fixed hight
    void freshDstImg(int alpha, int beta);//alpha: gain,contrast  beta: offset,brightness
};

#endif // LOCALBCDIALOG_H
