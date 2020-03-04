#ifndef CONSUMESTATUSDIALOG_H
#define CONSUMESTATUSDIALOG_H

#include <QDialog>
#include <memory>

class IDev;

namespace Ui {
class consumeStatusDialog;
}

class consumeStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit consumeStatusDialog(QWidget *parent = nullptr);
    ~consumeStatusDialog();

private slots:
    void on_pbtn_resetRollerNum_clicked();

private:
    Ui::consumeStatusDialog *ui;
//    std::shared_ptr<IDev> m_scanner;
};

#endif // CONSUMESTATUSDIALOG_H
