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
        QMessageBox::warning(nullptr,QStringLiteral("警告 "),QStringLiteral("程序已在运行！ " ));
        return 0;
    }
    else
        sm.create(1);
    MainWindow w;
    w.show();

    return a.exec();
}
