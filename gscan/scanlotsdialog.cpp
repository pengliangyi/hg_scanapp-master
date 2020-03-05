#include "scanlotsdialog.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

scanLotsDialog::scanLotsDialog(QWidget *parent) :
    QDialog(parent),
    a(new QFileDialog(this, Qt::SubWindow)),
    box(new QComboBox(this))
{
    QVBoxLayout* l = new QVBoxLayout(this);
    a->setNameFilter("*.jpg");
    a->setOption(QFileDialog::DontUseNativeDialog);
    a->setAcceptMode(QFileDialog::AcceptSave);
    a->setFileMode(QFileDialog::Directory);

    l->addWidget(a);
    QStringList lst;
    lst<<".jpg"<<".jpeg"<<".png"<<".bmp"<<".tif"<<".tif[multi]"<<".pdf"<<".pdf[multi]"<<".ofd"<<".ofd[multi]";
    box->addItems(lst);

    QLabel* lab = new QLabel(this);
    lab->setText(tr("选择要保存的文件格式:"));

    QHBoxLayout* f = new QHBoxLayout(nullptr);
    QSpacerItem* s = new QSpacerItem(100,50);
    f->addSpacerItem(s);
    f->addWidget(lab);
    f->addWidget(box);
    f->setStretch(0,1);

    l->addLayout(f);

    this->setLayout(l);

    connect(a, SIGNAL(accepted()), this, SLOT(close()));
    connect(a, SIGNAL(rejected()), this, SLOT(close()));
    connect(box,SIGNAL(currentTextChanged(QString)),this,SLOT(filterChange(QString)));
}

scanLotsDialog::~scanLotsDialog()
{
}

int scanLotsDialog::result()
{
    return a->result();
}

QDir scanLotsDialog::directory() const
{
    return a->directory();
}

QString scanLotsDialog::format() const
{
    return box->currentText();
}

void scanLotsDialog::filterChange(QString filter)
{
    if(filter == ".tif[multi]" || filter == ".pdf[multi]" || filter == ".ofd[multi]")
        filter = filter.remove("[multi]");
    a->setNameFilter("*"+filter);
}
