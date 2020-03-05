#include "AdminDialog.h"
#include "ui_AdminDialog.h"
#include <QMessageBox>

AdminDialog::AdminDialog(QWidget *parent, bool isAlreadyLogin) :
    QDialog(parent),
    ui(new Ui::AdminDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit->setPlaceholderText(tr("输入管理员密码 "));
    this->setFixedSize(350,120);
    init_function_usable();
    if(isAlreadyLogin)
        set_status_login();
}

AdminDialog::~AdminDialog()
{
    delete ui;
}

void AdminDialog::on_pbtn_login_clicked()
{
    if(ui->lineEdit->text() == "hg123123")
    {
        QMessageBox::information(this,tr("信息 "),tr("已登录为管理员。 "));
        ui->lab_loginStatus->setText(tr("管理员 "));
        ui->pbtn_login->setEnabled(false);
        ui->pbtn_logout->setEnabled(true);
        ui->lineEdit->clear();
    }
    else
    {
        QMessageBox::warning(this,tr("错误 "),tr("密码输入错误，请重新输入。 "));
    }
}
void AdminDialog::on_pbtn_logout_clicked()
{
    int a = QMessageBox::question(this,
                                  tr("询问 "),
                                  tr("退出管理员将导致无法使用日志相关操作，是否确认退出？ "),
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::No);
    if(a == QMessageBox::Yes)
    {
        ui->lab_loginStatus->setText(tr("未登录 "));
        ui->lineEdit->clear();
        ui->pbtn_login->setEnabled(true);
        ui->pbtn_logout->setEnabled(false);
    }
}
void AdminDialog::on_pbtn_exit_clicked()
{
    if(ui->pbtn_login->isEnabled())
        this->reject();
    else
        this->accept();
}

void AdminDialog::init_function_usable()
{
    ui->lab_loginStatus->setText(tr("未登录 "));
    ui->pbtn_logout->setEnabled(false);
}

void AdminDialog::set_status_login()
{
    ui->lab_loginStatus->setText(tr("管理员 "));
    ui->pbtn_login->setEnabled(false);
    ui->pbtn_logout->setEnabled(true);
}
