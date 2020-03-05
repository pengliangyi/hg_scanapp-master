#include "mainwindow.h"
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory sm("HuaGoScan");
    bool bIsRunning = sm.attach();
    if(bIsRunning)
    {
        QMessageBox::warning(nullptr,QObject::tr("warning"),QObject::tr("Application already run!" ));
        return 0;
    }
    else
        sm.create(1);
    MainWindow w;
    w.show();

    return a.exec();
}
