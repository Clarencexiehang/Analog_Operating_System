#include "processtab.h"
#include "ui_processtab.h"
#include "vector"
#include "time.h"
#include "algorithm"
#include "QDebug"
#include "QFont"
#include "QBrush"
#include "processdialog.h"



ProcessTab::ProcessTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessTab)
{
    ui->setupUi(this);

    //进程表格
    ui->processtable->setColumnCount(5);
    QStringList tableheader;
    tableheader<<"进程标识码"<<"优先级"<<"占用CPU时间"<<"还需CPU时间"<<"进程状态";
    ui->processtable->horizontalHeader()->setMinimumHeight(50);
    ui->processtable->setHorizontalHeaderLabels(tableheader);
    ui->processtable->verticalHeader()->setVisible(false);
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
        ui->processtable->setItem(i,0,new QTableWidgetItem(QString::fromStdString( readyQueue[i]->name)));
        ui->processtable->setItem(i,1,new QTableWidgetItem(QString::number(readyQueue[i]->prio)));
        ui->processtable->setItem(i,2,new QTableWidgetItem(QString::number(readyQueue[i]->cpuTime)));
        ui->processtable->setItem(i,3,new QTableWidgetItem(QString::number(readyQueue[i]->needTime)));
        ui->processtable->setItem(i,4,new QTableWidgetItem(QString::fromStdString(readyQueue[i]->state)));

        for(int j=0;j<5;j++){
            ui->processtable->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->processtable->item(i,j)->setBackground(QBrush(QColor(255,244,196)));
        }
    }
}

void ProcessTab::Random_Create_PCB(){
    for(int i=0;i<8;i++){
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
    process->needTime = rand()%50;
    process->prio = maxPrio - process->needTime;
    process->round = 5;
    process->cpuTime = 0;
    strcpy(process->state, "就绪");    //默认创建的新进程是就绪状态
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
    ui->processtable->setRowCount(0);
    readyQueue.clear();
    Random_Create_PCB();
    showProcess();
}


void ProcessTab::on_pause_clicked(bool checked)
{
    if(checked){
        ui->pause->setStyleSheet("background-color:rgb(253,0,0)");
    }else{
        ui->pause->setStyleSheet("background-color:rgb(253,253,253)");
    }

}
