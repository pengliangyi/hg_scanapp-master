#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QString version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(488,300);
    ui->label_2->setText("HuaGoScan Scanner Device Application.\nNingbo Huagao information and technology co.,ltd.\nVersion:"+version);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::on_buttonBox_accepted()
{
    close();
}
