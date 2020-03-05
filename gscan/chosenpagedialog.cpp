#include "chosenpagedialog.h"
#include "ui_chosenpagedialog.h"
#include "mainwindow.h"
#include <QCloseEvent>

chosenPageDialog::chosenPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chosenPageDialog)
{
    ui->setupUi(this);
    ui->spin_chosenPage->setValue(chosen_page);//get lastest setted pagenum
	ui->spin_chosenPage->setMaximum(count);
}

chosenPageDialog::~chosenPageDialog()
{
    delete ui;
}

void chosenPageDialog::setParam(int chosenPage, int listCount)
{
    chosen_page = chosenPage;
    count = listCount;
}

void chosenPageDialog::accept()
{
    chosen_page = ui->spin_chosenPage->value();
    close();
}
