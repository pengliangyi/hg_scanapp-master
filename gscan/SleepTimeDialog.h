#ifndef SLEEPTIMEDIALOG_H
#define SLEEPTIMEDIALOG_H

#include <QDialog>
#include <memory>
//#include <IGScan.h>
#include <QMap>

namespace Ui {
class SleepTimeDialog;
}

enum SLEEP_TIME
{
    MIN_5   = 300,
    MIN_10  = 600,
    MIN_20  = 1200,
    MIN_30  = 1800,
    HOUR_1  = 3600,
    HOUR_2  = 7200,
    HOUR_4  = 14400,
    NEVER   = -1
};

class SleepTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SleepTimeDialog(QWidget *parent = nullptr);
    ~SleepTimeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SleepTimeDialog *ui;
//    std::shared_ptr<IGScan> m_scanner;
    QMap<int,int> m_map;
};

#endif // SLEEPTIMEDIALOG_H
