#ifndef PROCESSTAB_H
#define PROCESSTAB_H

#include <QWidget>
#include "pcb.h"
#include "QPainter"
#include "vector"
#include "time.h"
#include "algorithm"
#include "QDebug"
#include "QFont"
#include "QBrush"
#include "processdialog.h"
#include "QMessageBox"
#include "QStringListModel"
#include "QTimer"
#include <QElapsedTimer>
#include <QVector>

#include<queue>
#include<cmath>
#include "QTabWidget"
#include "vector"
#include"cstring"
#include "algorithm"
namespace Ui {
class ProcessTab;
}

class ProcessTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessTab(QWidget *parent = nullptr);
    ~ProcessTab();

    vector<PCB*> processQueue;
    vector<PCB*> readyQueue;
    vector<PCB*> runningQueue;
    vector<PCB*> blockQueue;

    const int maxPrio=50;
    static bool compare(PCB* a,PCB* b);
    static bool cmp_needtime(PCB* a,PCB* b);
    void Random_Create_PCB();
    void insertReadyQueue(PCB* process,string name);
    void FCFS();
    void SJF();
    void Dynamic_Priority_Time_Slice_Rotation();        //动态优先级时间片轮转法
    void showQueue();
    void showVisitPages(PCB *process);
    void updateTableWidget(PCB *runOne,string state);

    int semaphore_keyboard = 1;
    int semaphore_full = 0;
    int mutex = 1;
    bool isUsed1 = false;
    bool isUsed2 = false;
    bool P(int &semaphore);
    bool V(int &semaphore);
    bool Process_Behaviour(string behaviour,PCB* runOne);
    void Create_Process_For_Synchronization();

    void Create_Process_For_File(QVector<int> disk_visit_Queue);



private slots:
    void on_createprocess_clicked();
    void on_deleteprocess_clicked();
    void on_start_clicked();
    void on_reset_clicked();

public slots:
    void showProcess();



private:
    Ui::ProcessTab *ui;
    QTimer * timer;
    QElapsedTimer t;
    QElapsedTimer t1;
};

#endif // PROCESSTAB_H
