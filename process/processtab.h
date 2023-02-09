#ifndef PROCESSTAB_H
#define PROCESSTAB_H

#include <QWidget>
#include "pcb.h"

namespace Ui {
class ProcessTab;
}

class ProcessTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessTab(QWidget *parent = nullptr);
    ~ProcessTab();

    vector<PCB*> readyQueue;
    vector<PCB*> runningQueue;
    vector<PCB*> finishQueue;

    const int maxPrio=50;
    static bool compare(PCB* a,PCB* b);
    void Random_Create_PCB();
    void insertReadyQueue(PCB* process,string name);
    void FCFS();
    void Dynamic_Priority_Time_Slice_Rotation(PCB* process);        //动态优先级时间片轮转法

    QString test = "P!!!";


private slots:
    void on_createprocess_clicked();
    void on_deleteprocess_clicked();
    void on_start_clicked();
    void on_pause_clicked(bool checked);

public slots:
    void showProcess();


private:
    Ui::ProcessTab *ui;
};

#endif // PROCESSTAB_H
