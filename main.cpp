#include "mainwindow.h"
#include <QApplication>
#include "process/pcb.h"
#include "vector"
#include "time.h"

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w=new MainWindow;


    w->show();
    return a.exec();
}
