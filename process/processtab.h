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
    void Random_Create_PCB();
    void insertReadyQueue(PCB* process,string name);
    void FCFS();
    void Dynamic_Priority_Time_Slice_Rotation(PCB* process);        //动态优先级时间片轮转法
    void showQueue();
    void showVisitPages(PCB *process);


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
    QTimer * timer;
    QElapsedTimer t;
};

#endif // PROCESSTAB_H
