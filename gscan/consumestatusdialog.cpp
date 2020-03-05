#include "consumestatusdialog.h"
#include "ui_consumestatusdialog.h"
#include <QMessageBox>
#include <thread>

consumeStatusDialog::consumeStatusDialog(QWidget *parent) :
    QDialog(parent),
//    m_scanner(scanner),
    ui(new Ui::consumeStatusDialog)
{
    ui->setupUi(this);
   /* ui->lab_currRollerNum->setText(QString::number(m_scanner->get_roller_scaned()));
    ui->lab_historyScanNum->setText(QString::number(m_scanner->get_roller_total_scaned()));*/
}

consumeStatusDialog::~consumeStatusDialog()
{
    delete ui;
}

void consumeStatusDialog::on_pbtn_resetRollerNum_clicked()
{
    int ret = QMessageBox::question(this,
                                    tr("重要 "),
                                    tr("是否确认要重置当前辊轴已扫描总数？\n""(此操作仅重置辊轴已扫描张数，不会清除历史扫描总数) "),
                                    QMessageBox::Ok | QMessageBox::Cancel,
                                    QMessageBox::Cancel);
    switch(ret)
    {
    case QMessageBox::Ok:
      /*  m_scanner->clr_roller_scaned();
        if(m_scanner->get_roller_scaned())
        {
            QMessageBox::warning(this,tr("警告 "),tr("重置辊轴已扫描总数失败，请重试！ "));
        }
        else
        {
            ui->lab_currRollerNum->setText("0");
        }*/
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}
