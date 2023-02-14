#include "processtab.h"
#include "ui_processtab.h"
#include "mainwindow.h"

extern MainWindow *w;

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
//    connect(timer,SIGNAL(timeout()),this,SLOT(on_pause_clicked(bool checked)));
//    timer->start(100);

}

ProcessTab::~ProcessTab()
{
    delete ui;
}

//展示三个队列的情况
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
    for (int i=0;i<blockQueue.size();i++) {
        blockProcess<<QString::fromStdString(blockQueue[i]->name);
    }
    ui->listWidget_block->addItems(blockProcess);

    QCoreApplication::processEvents();
}

//展示要访问的页面
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


//更新表格状态
void ProcessTab::updateTableWidget(PCB *runOne,string state){
    for(unsigned int i=0;i<processQueue.size();i++){
        if(processQueue[i]->name==runOne->name){
            if(state == "完成"){
                strcpy( processQueue[i]->state, "完成");
                processQueue[i]->needTime = 0;
                w->memoryTab->freeMemery(QString::fromStdString(processQueue[i]->name));
            }else if (state == "运行"){
                strcpy( processQueue[i]->state, "运行");
                processQueue[i]->needTime = runOne->needTime;

            }else if (state == "round"){
                 strcpy(processQueue[i]->state, "就绪");
                 processQueue[i]->prio = runOne->prio;
                 processQueue[i]->needTime = runOne->needTime;
                 processQueue[i]->cpuTime = runOne->cpuTime;
            }
        }
    }
    this->showProcess();
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

//随机创建进程
void ProcessTab::Random_Create_PCB(){
    for(int i=0;i<3;i++){
        PCB* p = nullptr;
        string name = "pro"+to_string(i);
        insertReadyQueue(p,name);
    }
}

void ProcessTab::insertReadyQueue(PCB* process,string name){
    process = new PCB(name);

    //放入就绪队列
    processQueue.push_back(process);
    readyQueue.push_back(process);

//    //分配内存
//    QString p = QString::fromStdString(process->name);
//    if(process->needTime>10){
//        w->memoryTab->requestMemery(5,QString::fromStdString(process->name));
//    }else{
//        int pageFrame = rand()%4+1;
//        w->memoryTab->requestMemery(pageFrame,QString::fromStdString(process->name));
//    }
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
    for(int i=0;i<processQueue.size();i++){
        if(QString::fromStdString(processQueue[i]->name) == ui->processtable->item(currentRow,0)->text()){
            processQueue.erase(processQueue.begin()+i);
        }
    }
    if(!readyQueue.empty()){
        for(unsigned int i=0;i<readyQueue.size();i++){
            if(QString::fromStdString(readyQueue[i]->name) == ui->processtable->item(currentRow,0)->text()){
                readyQueue.erase(readyQueue.begin()+i);
            }
        }
    }else{
        ui->processtable->removeRow(currentRow);
        return ;
    }
//    qDebug()<<"pid:"<<ui->processtable->item(currentRow,0)->text();
    w->memoryTab->freeMemery(ui->processtable->item(currentRow,0)->text());
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
        this->SJF();
    }else if(ui->DPTSR->isChecked()){
        this->Dynamic_Priority_Time_Slice_Rotation();
    }else{
        QMessageBox::warning(this,"错误提示",tr("请选择调度算法"));
        return ;
    }


}


//重置按钮
void ProcessTab::on_reset_clicked()
{
    for (int i=0;i<processQueue.size();i++) {
        if(strcmp(processQueue[i]->state,"完成")!=0){
            w->memoryTab->freeMemery(QString::fromStdString( processQueue[i]->name));
        }
    }
    readyQueue.clear();
    processQueue.clear();
    Random_Create_PCB();
    Create_Process_For_Synchronization();
    showProcess();
}




/**************************************先来先服务法********************************************/
void ProcessTab::FCFS(){
h:
    while(!readyQueue.empty() || !blockQueue.empty() || !runningQueue.empty()){
        PCB *runOne = readyQueue[0];    //qDebug()<<"ready"<<readyQueue.size();
        readyQueue.erase(readyQueue.begin());
        runningQueue.push_back(runOne);
        this->showQueue();
        this->showVisitPages(runOne);

        //判断进程是否有事件，进行PV操作
        bool flag;
        if(runOne->behaviour == "键盘输入" && !isUsed1){
            isUsed1 = true;
            flag = this->Process_Behaviour("键盘输入",runOne);
            if(flag){
                goto h;
            }

        }else if(runOne->behaviour == "获取键值" && !isUsed2){
            isUsed2 = true;
            flag = this->Process_Behaviour("获取键值",runOne);
            if(flag){
                goto h;
            };
        }

        while(runOne->needTime>0){

            runOne->needTime--;
            runOne->cpuTime++;

            //更新进程信息，更新表格状态
            this->updateTableWidget(runOne,"运行");
            w->memoryTab->replacePageByLRU(QString::fromStdString(runOne->name),runOne->visit_pages[runOne->visit_page_index]);


            //更新输出日志
            ui->textBrowser->insertPlainText("正在执行进程"+QString::fromStdString(runOne->name)+", 访问页面:"+QString::number(runOne->visit_pages[runOne->visit_page_index++])
                                              +",  已运行时间:"+ QString::number(runOne->cpuTime));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));

            //延时1s
            t.start();
            while(t.elapsed()<1000);
        }

        //进程执行完后更新
        this->updateTableWidget(runOne,"完成");
        runningQueue.pop_back();
        this->showQueue();

        ui->textBrowser->insertPlainText("进程 "+QString::fromStdString(runOne->name)+" 执行完毕！！！");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));

        if(runOne->behaviour == "键盘输入"){
            V(mutex);
            flag = V(semaphore_full);
        }else if(runOne->behaviour == "获取键值"){
            V(this->mutex);
            V(semaphore_keyboard);
        }
    }
}


/**********************************短作业优先法*******************************/
//按照优先级从大到小排序
bool ProcessTab::cmp_needtime(PCB *a, PCB *b){
    return a->needTime < b->needTime;
}

void ProcessTab::SJF(){
h:    while(!readyQueue.empty() || !blockQueue.empty() || !runningQueue.empty()){
        sort(readyQueue.begin(),readyQueue.end(),ProcessTab::cmp_needtime);  //就绪队列排序
        PCB *runOne = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        runningQueue.push_back(runOne);
        this->showQueue();
        this->showVisitPages(runOne);

        //判断进程是否有事件，进行PV操作
        bool flag;
        if(runOne->behaviour == "键盘输入" && !isUsed1){
            isUsed1 = true;
            flag = this->Process_Behaviour("键盘输入",runOne);
            if(flag){
                goto h;
            }

        }else if(runOne->behaviour == "获取键值" && !isUsed2){
            isUsed2 = true;
            flag = this->Process_Behaviour("获取键值",runOne);
            if(flag){
                goto h;
            };
        }

        while(runOne->needTime>0){
            runOne->needTime--;
            runOne->cpuTime++;

            //更新进程信息，更新表格状态
            this->updateTableWidget(runOne,"运行");
            w->memoryTab->replacePageByLRU(QString::fromStdString(runOne->name),runOne->visit_pages[runOne->visit_page_index]);


            //更新输出日志
            ui->textBrowser->insertPlainText("正在执行进程"+QString::fromStdString(runOne->name)+", 访问页面:"+QString::number(runOne->visit_pages[runOne->visit_page_index++])
                                              +",  已运行时间:"+ QString::number(runOne->cpuTime));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));

            //延时1s
            t.start();
            while(t.elapsed()<1000);
        }
        //进程执行完后更新
        this->updateTableWidget(runOne,"完成");
        runningQueue.pop_back();
        this->showQueue();

        ui->textBrowser->insertPlainText("进程 "+QString::fromStdString(runOne->name)+" 执行完毕！！！");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));

        if(runOne->behaviour == "键盘输入"){
            V(mutex);
            flag = V(semaphore_full);
        }else if(runOne->behaviour == "获取键值"){
            V(this->mutex);
            V(semaphore_keyboard);
        }
    }
}


/**********************************动态优先级时间片轮转法*******************************/
//按照优先级从大到小排序
bool ProcessTab::compare(PCB* a,PCB* b){
    return a->prio > b->prio;
}


void ProcessTab::Dynamic_Priority_Time_Slice_Rotation(){
h:  while(!readyQueue.empty() || !blockQueue.empty() || !runningQueue.empty()){
        sort(readyQueue.begin(),readyQueue.end(),ProcessTab::compare);  //就绪队列排序
        PCB *runOne = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        strcpy(runOne->state, "运行");
        runningQueue.push_back(runOne);
        this->showQueue();
        this->showVisitPages(runOne);


        //判断进程是否有事件，进行PV操作
        bool flag;
        if(runOne->behaviour == "键盘输入" && !isUsed1){
            isUsed1 = true;
            flag = this->Process_Behaviour("键盘输入",runOne);
            if(flag){
                goto h;
            }

        }else if(runOne->behaviour == "获取键值" && !isUsed2){
            isUsed2 = true;
            flag = this->Process_Behaviour("获取键值",runOne);
            if(flag){
                goto h;
            };
        }


        //如果进程的所需执行时间<时间片，则进程执行完毕
        if(runOne->needTime<=runOne->round){
            runOne->cpuTime += runOne->needTime;
            runOne->needTime=0;
            qDebug()<<"进程 "<<QString::fromStdString(runOne->name)<<" 执行完毕！";
            this->updateTableWidget(runOne,"完成");

            runningQueue.pop_back();
            this->showQueue();

            ui->textBrowser->insertPlainText("进程 "+QString::fromStdString(runOne->name)+" 执行完毕！！！");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));

            if(runOne->behaviour == "键盘输入"){
                V(mutex);
                flag = V(semaphore_full);
            }else if(runOne->behaviour == "获取键值"){
                V(this->mutex);
                V(semaphore_keyboard);
            }

        }
        //如果进程没有执行完
        else{
            runOne->needTime -= runOne->round;
            runOne->cpuTime += runOne->round;

            this->updateTableWidget(runOne,"运行");
            w->memoryTab->replacePageByLRU(QString::fromStdString(runOne->name),runOne->visit_pages[runOne->visit_page_index]);
            this->showQueue();

            //更新输出日志
            ui->textBrowser->insertPlainText("正在执行进程"+QString::fromStdString(runOne->name)+", 访问页面:"+QString::number(runOne->visit_pages[runOne->visit_page_index++])
                                              +", 优先级:"+QString::number(runOne->prio)+",  已运行时间:"+ QString::number(runOne->cpuTime));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            QCoreApplication::processEvents();
            //延时1s
            t.start();
            while(t.elapsed()<1000);

            for(unsigned int i=0;i< readyQueue.size();i++){
                readyQueue[i]->prio+=1;     //就绪队列中所有进程优先级提高1
            }
            if(runOne->prio!=1){
                runOne->prio = runOne->prio-1;
            }

             //将时间片轮转的进程重新放入就绪队列中
            strcpy(runOne->state, "就绪");
            runningQueue.clear();
            readyQueue.push_back(runOne);
            this->showQueue();
            this->updateTableWidget(runOne,"round");

            //更新输出日志
            ui->textBrowser->insertPlainText("进程 "+QString::fromStdString(runOne->name)+"优先级降低为:"+QString::number(runOne->prio));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            QCoreApplication::processEvents();
            //延时1s,轮转效果
            t1.start();
            while(t1.elapsed()<1000);
        }
    }
}


/**************************************** 信号量模拟进程同步 *************************************************/
bool ProcessTab::P(int &semaphore){
    semaphore--;                //qDebug()<<"P--full:"<<semaphore;
    if(semaphore<0){
        PCB* tempProcess = runningQueue[0];
        runningQueue.clear();
        strcpy(tempProcess->state, "阻塞");
        blockQueue.push_back(tempProcess);
        return true;
    }
    return false;
}

bool ProcessTab::V(int &semaphore){
    semaphore++;

    //唤醒一个进程
    if(semaphore<=0){
        semaphore++;
        PCB* tempProcess = blockQueue[0];
        blockQueue.erase(blockQueue.begin());
        strcpy(tempProcess->state, "就绪");
        readyQueue.push_back(tempProcess);
        return true;
    }
    return  false;
}

//模拟进程的行为
bool ProcessTab::Process_Behaviour(string behaviour,PCB* runOne){
    bool flag;
    if(behaviour == "键盘输入"){
        flag = P(this->semaphore_keyboard);
        P(this->mutex);

        //日志
        ui->textBrowser->insertPlainText("键盘正在输入...");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        QCoreApplication::processEvents();
        return flag;
    }

    else if(behaviour == "获取键值"){
        flag = P(semaphore_full);

        if(flag == true){
            return flag;
        }

        P(this->mutex);
        //日志
        ui->textBrowser->insertPlainText("正在获取键值...");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        QCoreApplication::processEvents();

        return flag;
    }

}

//创建进程模拟进程同步
void ProcessTab::Create_Process_For_Synchronization(){
    PCB* p2 = new PCB("获取键值");
    p2->behaviour = "获取键值";
    //放入就绪队列
    processQueue.push_back(p2);
    readyQueue.push_back(p2);



    PCB* p1 = new PCB("键盘输入");
    p1->behaviour = "键盘输入";
    //放入就绪队列
    processQueue.push_back(p1);
    readyQueue.push_back(p1);
}



/**************************************** 磁盘调度访问磁道号顺序 *************************************************/
void ProcessTab::Create_Disk_Track(){
    PCB* p1 = new PCB("访问磁盘");
    p1->equip = "磁盘";
    //放入就绪队列
    processQueue.push_back(p1);
    readyQueue.push_back(p1);

    for(int i=0;i<p1->needTime;i++){
        p1->visit_pages[i] = rand()%20;
        p1->disk_track[i]=rand()%20;
    }
}
