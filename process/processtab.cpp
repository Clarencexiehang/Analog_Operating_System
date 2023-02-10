#include "processtab.h"
#include "ui_processtab.h"

#include "mainwindow.h"


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

    //队列外观
    ui->listWidget_ready->setViewMode(QListView::ListMode);
    ui->listWidget_ready->setAlternatingRowColors(true);

    //访问页面外观
    ui->listWidget->setViewMode(QListView::IconMode);


    //定时器
//    timer = new QTimer(this);
//   connect(timer,SIGNAL(timeout()),this,SLOT(FCFS()));

}

ProcessTab::~ProcessTab()
{
    delete ui;
}

void ProcessTab::showQueue(){
    ui->listWidget_ready->clear();
    ui->listWidget_running->clear();
    ui->listWidget_block->clear();
    //就绪队列
    QStringList readyProcess;
    for (unsigned int i=0;i<readyQueue.size();i++) {
        readyProcess<<QString::fromStdString(readyQueue[i]->name);
    }
    ui->listWidget_ready->addItems(readyProcess);

    //运行队列
    QStringList runProcess;
    for (unsigned int i=0;i<runningQueue.size();i++) {
        runProcess<<QString::fromStdString(runningQueue[i]->name);
    }
    ui->listWidget_running->addItems(runProcess);

    //阻塞队列
    QStringList blockProcess;
    for (int i=0;i<blockProcess.size();i++) {
        blockProcess<<QString::fromStdString(blockQueue[i]->name);
    }
    ui->listWidget_block->addItems(blockProcess);

    QCoreApplication::processEvents();
}

void ProcessTab::showVisitPages(PCB *process){
    ui->listWidget->clear();
    QStringList visitpage;
    int num = process->needTime;
    for (int i=0;i<num;i++) {
        visitpage<<QString::number(process->visit_pages[i]);
    }

    ui->listWidget->addItems(visitpage);
    QCoreApplication::processEvents();
}


//展示进程
void ProcessTab::showProcess(){
    ui->processtable->setRowCount(0);
    for(unsigned int i=0;i<processQueue.size();i++){
        int nCount = ui->processtable->rowCount();
        ui->processtable->insertRow(nCount);
        ui->processtable->setItem(int(i),0,new QTableWidgetItem(QString::fromStdString( processQueue[i]->name)));
        ui->processtable->setItem(int(i),1,new QTableWidgetItem(QString::number(processQueue[i]->prio)));
        ui->processtable->setItem(int(i),2,new QTableWidgetItem(QString::number(processQueue[i]->cpuTime)));
        ui->processtable->setItem(int(i),3,new QTableWidgetItem(QString::number(processQueue[i]->needTime)));
        ui->processtable->setItem(int(i),4,new QTableWidgetItem(QString::fromStdString(processQueue[i]->equip)));
        ui->processtable->setItem(int(i),5,new QTableWidgetItem(QString::fromStdString(processQueue[i]->state)));

        for(int j=0;j<6;j++){
            ui->processtable->item(int(i),j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->processtable->item(int(i),j)->setBackground(QBrush(QColor(255,244,196)));
            if(strcmp(processQueue[i]->state,"运行")==0){
                ui->processtable->item(int(i),j)->setBackground(QBrush(QColor(255,210,100)));
            }
        }
    }
    QCoreApplication::processEvents();
}

void ProcessTab::Random_Create_PCB(){
    for(int i=0;i<3;i++){
        PCB* p = nullptr;
        string name = "pro"+to_string(i);
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
    process->needTime = rand()%9+1;
    process->prio = 10 - process->needTime;
    process->round = 1;
    process->cpuTime = 0;
    strcpy(process->state, "就绪");    //默认创建的新进程是就绪状态
    process->equip = "无";
    process->visit_page_index = 0;

    for(int i=0;i<process->needTime;i++){
        process->visit_pages[i] = rand()%10;
    }
    //放入就绪队列
    processQueue.push_back(process);
    readyQueue.push_back(process);

    extern MainWindow *w;
//    string a="1";
//    qDebug()<<std::stoi(a.c_str());
//    w->memoryTab->requestMemery(5,stoi(process->name.c_str()));

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
    //判断使用哪种调度方法
    if (ui->FCFS->isChecked()) {
        // timer->start(1000); //启动定时器，运行完一个进程
        FCFS();
    }else if(ui->SJF->isChecked()){

    }else if(ui->DPTSR->isChecked()){
//        this->Dynamic_Priority_Time_Slice_Rotation();
    }else{
        QMessageBox::warning(this,"错误提示",tr("请选择调度算法"));
        return ;
    }


}

//暂停按钮
void ProcessTab::on_pause_clicked(bool checked)
{
    if(checked){
        ui->pause->setStyleSheet("background-color:rgb(255,0,0)");
        while(checked){
            ui->textBrowser->insertPlainText("暂停！");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            if(!checked){
                 ui->pause->setStyleSheet("background-color:rgb(253,253,253)");
            }
        }
    }else{
        ui->pause->setStyleSheet("background-color:rgb(253,253,253)");
    }

}


/**************************************先来先服务法********************************************/
void ProcessTab::FCFS(){
    while(!readyQueue.empty() || !blockQueue.empty() || !runningQueue.empty()){
        PCB *runOne = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        runningQueue.push_back(runOne);
        this->showQueue();
        this->showVisitPages(runOne);

        int usetime=1;
        while(runOne->needTime>0){
            runOne->needTime--;

            //更新进程信息，更新表格状态
            for(unsigned int i=0;i<processQueue.size();i++){
                if(processQueue[i]->name==runOne->name){
                    strcpy( processQueue[i]->state, "运行");
                    processQueue[i]->needTime = runOne->needTime;
                    processQueue[i]->cpuTime = usetime++;
                   // processQueue[i]->visit_page_index++;
                }
            }
            this->showProcess();

            //更新输出日志
            ui->textBrowser->insertPlainText("正在执行进程"+QString::fromStdString(runOne->name)+", 访问页面:"+QString::number(runOne->visit_pages[runOne->visit_page_index++])
                                              +",  已运行时间:"+ QString::number(runOne->cpuTime));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));

            //延时1s
            t.start();
            while(t.elapsed()<1000);
        }

        runningQueue.pop_back();
        this->showQueue();
        //更新表格状态
        for(unsigned int i=0;i<processQueue.size();i++){
            if(processQueue[i]->name==runOne->name){
                strcpy( processQueue[i]->state, "完成");
                processQueue[i]->needTime = 0;
            }
        }
        this->showProcess();

        ui->textBrowser->insertPlainText("进程 "+QString::fromStdString(runOne->name)+" 执行完毕！！！");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));

        qDebug()<<"yes";
    }
}


/**********************************短作业优先法*******************************/
void ProcessTab::SJF(){
    while(!readyQueue.empty() || !blockQueue.empty() || !runningQueue.empty()){
        PCB *runOne = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        runningQueue.push_back(runOne);
        this->showQueue();
        this->showVisitPages(runOne);
    }
}


/**********************************动态优先级时间片轮转法*******************************/
void ProcessTab::Dynamic_Priority_Time_Slice_Rotation(PCB* process){
    sort(readyQueue.begin(),readyQueue.end(),compare);  //就绪队列排序
    runningQueue.erase(runningQueue.begin());   //清空运行队列

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
        //finishQueue.push_back(process);

        qDebug()<<"进程 "<<QString::fromStdString(process->name)<<" 执行完毕！";

    }else{
        process->prio = tempPrio-2;
        strcpy(process->state, "ready");
        readyQueue.push_back(process);
        sort(readyQueue.begin(),readyQueue.end(), compare);
    }
}

