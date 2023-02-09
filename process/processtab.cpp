#include "processtab.h"
#include "ui_processtab.h"
#include "vector"
#include "time.h"
#include "algorithm"
#include "QDebug"
#include "QFont"
#include "QBrush"
#include "processdialog.h"
#include "QMessageBox"
#include "QStringListModel"

ProcessTab::ProcessTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessTab)
{
    ui->setupUi(this);

    //进程表格
    ui->processtable->setColumnCount(6);
    QStringList tableheader;
    tableheader<<"进程标识码"<<"优先级"<<"运行时间"<<"所需时间"<<"使用设备"<<"进程状态";
    ui->processtable->horizontalHeader()->setMinimumHeight(50);
    ui->processtable->setHorizontalHeaderLabels(tableheader);
    ui->processtable->verticalHeader()->setVisible(false);
//    ui->processtable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->processtable->setAlternatingRowColors(true);
    ui->processtable->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选择每行且单元不可编辑
    ui->processtable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->processtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->processtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}

ProcessTab::~ProcessTab()
{
    delete ui;
}


//展示进程
void ProcessTab::showProcess(){
    ui->processtable->setRowCount(0);
    for(unsigned int i=0;i<readyQueue.size();i++){
        int nCount = ui->processtable->rowCount();
        ui->processtable->insertRow(nCount);
        ui->processtable->setItem(int(i),0,new QTableWidgetItem(QString::fromStdString( readyQueue[i]->name)));
        ui->processtable->setItem(int(i),1,new QTableWidgetItem(QString::number(readyQueue[i]->prio)));
        ui->processtable->setItem(int(i),2,new QTableWidgetItem(QString::number(readyQueue[i]->cpuTime)));
        ui->processtable->setItem(int(i),3,new QTableWidgetItem(QString::number(readyQueue[i]->needTime)));
        ui->processtable->setItem(int(i),4,new QTableWidgetItem(QString::fromStdString(readyQueue[i]->equip)));
        ui->processtable->setItem(int(i),5,new QTableWidgetItem(QString::fromStdString(readyQueue[i]->state)));

        for(int j=0;j<6;j++){
            ui->processtable->item(int(i),j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->processtable->item(int(i),j)->setBackground(QBrush(QColor(255,244,196)));
        }
    }
}

void ProcessTab::Random_Create_PCB(){
    for(int i=0;i<3;i++){
        PCB* p = nullptr;
        string name = "process"+to_string(i);
        insertReadyQueue(p,name);
    }
}

//按照优先级从大到小排序
bool ProcessTab::compare(PCB* a,PCB* b){
    return a->prio > b->prio;
}

void ProcessTab::insertReadyQueue(PCB* process,string name){
    process = new PCB;

    process->name = name;
    process->needTime = rand()%10;
    process->prio = 10 - process->needTime;
    process->round = 1;
    process->cpuTime = 0;
    strcpy(process->state, "就绪");    //默认创建的新进程是就绪状态
    process->equip = "无";
    process->visit_page_index = 0;

    for(int i=0;i<10;i++){
        process->visit_pages[i] = rand()%10;
    }
    //根据优先级排序，放入就绪队列
    readyQueue.push_back(process);
    sort(readyQueue.begin(),readyQueue.end(),compare);
}


//创建进程按钮槽
void ProcessTab::on_createprocess_clicked()
{
    processDialog *newProcessDialog = new processDialog;
    newProcessDialog->setAttribute(Qt::WA_DeleteOnClose);
    newProcessDialog->setWindowTitle("创建进程");

    newProcessDialog->show();

}

//删除表格按钮
void ProcessTab::on_deleteprocess_clicked()
{
    int currentRow = ui->processtable->currentRow();
    ui->processtable->removeRow(currentRow);
}

//开始按钮
void ProcessTab::on_start_clicked()
{
    //清空表格
    ui->processtable->setRowCount(0);

    //判断使用哪种调度方法
    if (ui->FCFS->isChecked()) {
        this->FCFS();
    }else if(ui->SJF->isChecked()){

    }else if(ui->DPTSR->isChecked()){

    }else{
        QMessageBox::warning(this,"错误提示",tr("请选择调度算法"));
        return ;
    }

    showProcess();
}

//暂停按钮
void ProcessTab::on_pause_clicked(bool checked)
{
    if(checked){
        ui->pause->setStyleSheet("background-color:rgb(255,0,0)");
    }else{
        ui->pause->setStyleSheet("background-color:rgb(253,253,253)");
    }

}


//先来先服务法
void ProcessTab::FCFS(){
    QStringList visitpage;
    for (int i=0;i<10;i++) {
        visitpage<<QString::number(readyQueue[0]->visit_pages[i]);
    }
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->addItems(visitpage);
}

//动态优先级时间片轮转法
void ProcessTab::Dynamic_Priority_Time_Slice_Rotation(PCB* process){
    runningQueue.erase(runningQueue.begin());

    int tempPrio;

    strcpy(process->state, "运行");
        //如果进程的所需执行时间<时间片，则优先级返回0
        if(process->needTime<process->round){
            process->cpuTime += process->needTime;
            process->needTime=0;
            tempPrio=0;
        } else{
            process->needTime -= process->round;
            process->cpuTime += process->round;
            tempPrio = process->prio;
            process->prio = maxPrio;     //运行时的进程优先级最大
        }

    //将时间片轮转的进程重新放入就绪队列中
    for(unsigned int i=0;i< readyQueue.size();i++){
        readyQueue[i]->prio+=2;     //就绪队列中所有进程优先级提高2
    }
    if(tempPrio == 0){
        strcpy(process->state, "finished");
        finishQueue.push_back(process);
        qDebug()<<"进程 "<<QString::fromStdString(process->name)<<" 执行完毕！";
    }else{
        process->prio = tempPrio-2;
        strcpy(process->state, "ready");
        readyQueue.push_back(process);
        sort(readyQueue.begin(),readyQueue.end(), compare);
    }
}



