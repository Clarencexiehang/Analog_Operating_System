#include "mainwindow.h"
#include <QApplication>
#include "pcb.h"
#include "vector"
#include "time.h"

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w=new MainWindow;

    //随机产生进程并展示
    srand(time(nullptr));
    w->processTab->Random_Create_PCB();
    w->processTab->showProcess();

    w->show();
    return a.exec();
}
