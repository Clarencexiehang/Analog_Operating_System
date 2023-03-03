#include "pcb.h"

#include "mainwindow.h"
extern MainWindow *w;

PCB::PCB(string name)
{
    this->name = name;
    this->needTime = rand()%19+1;
    this->prio = 20 - this->needTime;
    this->round = 2;
    this->cpuTime = 0;
    strcpy(this->state, "就绪");    //默认创建的新进程是就绪状态
    this->equip = "无";
    this->visit_page_index = 0;
    this->behaviour = "null";

    for(int i=0;i<this->needTime;i++){
        this->visit_pages[i] = rand()%20;
    }


    if(this->needTime>10){
        w->memoryTab->requestMemery(5,QString::fromStdString(this->name));
    }else{
        int pageFrame = rand()%4+1;
        w->memoryTab->requestMemery(pageFrame,QString::fromStdString(this->name));
    }
}
