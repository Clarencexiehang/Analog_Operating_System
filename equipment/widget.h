#ifndef WIDGET_H
#define WIDGET_H
#include <time.h>
#include <QWidget>
#include <QDebug>
#include "myprocess.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QAbstractTableModel>
#include "ui_myprocess.h"
#include "ui_dequip.h"
#include <QQueue>
#include "dequip.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    int totalPrint;
    int totalKeyboard;
    int totalMouse;
    int totalBluetooth;
    int rePrint;    //剩余
    int reKey;
    int reMouse;
    int reBlue;
    struct Process{
        int pId;
        int print;
        int keyboard;
        int mouse;
        int bluetooth;
        int getPrint;
        int getKey;
        int getMouse;
        int getBluetooth;
    };
    Process proc[24];
    QQueue <Process> wait;
    ~Widget();

private slots:
    void on_pro1_clicked();
    void on_createPro_clicked();
    void on_closeButton_clicked();

    void on_pro2_clicked();

    void on_pro3_clicked();

    void on_pro4_clicked();

    void on_pro5_clicked();

    void on_pro6_clicked();

    void on_pro7_clicked();

    void on_pro8_clicked();

    void on_pro9_clicked();

    void on_pro10_clicked();

    void on_pro11_clicked();

    void on_pro12_clicked();

    void on_pro13_clicked();

    void on_pro14_clicked();

    void on_pro15_clicked();

    void on_pro16_clicked();

    void on_pro17_clicked();

    void on_pro18_clicked();

    void on_pro19_clicked();

    void on_pro20_clicked();

    void on_pro21_clicked();

    void on_pro22_clicked();

    void on_pro23_clicked();

    void on_pro24_clicked();

    void on_StartButton_clicked();

    void waitAppendSlot (int pId);

private:
    Ui::Widget *ui;
    myProcess *mypro;

};

#endif // WIDGET_H
