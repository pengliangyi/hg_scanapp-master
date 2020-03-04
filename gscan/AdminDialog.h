#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDialog(QWidget *parent = nullptr, bool isAlreadyLogin = false);
    ~AdminDialog();

private slots:
    void on_pbtn_login_clicked();
    void on_pbtn_logout_clicked();
    void on_pbtn_exit_clicked();

private:
    Ui::AdminDialog *ui;
    void init_function_usable();
    void set_status_login();
};

#endif // ADMINDIALOG_H
