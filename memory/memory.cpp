#include "memory.h"
#include "ui_memory.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QMenuBar>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
extern MainWindow *w;
int color[10][3] = {
    {33,222,28},
    {188,133,240},
    {237,14,111},
    {240,20,138},
    {233,198,1},
    {233,94,1},
    {10,211,229},
    {180,10,229},
    {10,133,190},
    {9,154,135}
};
Memory::Memory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);
//    QMessageBox::information(this,"info","信息");
    int columnCount = 20;
    int rowCount = 20;
    ui->check1->setChecked(true);
    ui->memeryTable->setColumnCount(columnCount);
    ui->memeryTable->setRowCount(rowCount);
//    ui->processtable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->memeryTable->setAlternatingRowColors(true);
    ui->memeryTable->verticalHeader()->setDefaultSectionSize(5);
    ui->memeryTable->horizontalHeader()->setDefaultSectionSize(5);
    ui->memeryTable->verticalHeader()->setVisible(false);
    ui->memeryTable->horizontalHeader()->setVisible(false);
    ui->memeryTable->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选择每行且单元不可编辑
//    ui->memeryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->memeryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->memeryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i=0;i<columnCount;i++){
        for(int j=0;j<rowCount;j++){
            QTableWidgetItem  *tableitem = new QTableWidgetItem("");
            tableitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->memeryTable->setItem(i,j,tableitem);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(200,200,200)));

        }
    }
    this->initMemery();

    // 测试函数的代码

//    this->requestMemery(5,"22");
//    this->requestMemery(3,"21");
//    this->requestMemery(3,"23");
//    QTimer * timer = new QTimer(this);
//    QTimer * timer2 = new QTimer(this);
//    timer->start(500);
//    Memory * that = this;
//    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//    connect(timer,&QTimer::timeout,[=](){
//        that->freeMemery("21");
////        that->requestMemery(3,"23");
//        timer->stop();
//    });
//    timer2->start(1000);
//    connect(timer2,&QTimer::timeout,[=](){
//        int ran = rand() % 10;
//        that->freeMemery("22");
////        that->requestMemery(3,"24");
//        that->replacePageByLRU("21",ran);
//        timer2->stop();
//    });
//    that->requestMemery(2,"1");
//    that->requestMemery(7,"1");
}

Memory::~Memory()
{
    delete ui;
}
//初始化内存块的函数；
void Memory::initMemery(){
//    struct freeMemeryBlock * newBlock = new struct freeMemeryBlock();
//    newBlock->memeryBlockSize = 20*20;
//    newBlock->endIndex = 399;
//    newBlock->startIndex = 0;
//    newBlock->nextBlock = nullptr;
//    this->freeMemeryList = newBlock;
    for(int i=0;i<400;i++){
        this->isUsed[i] = 0;
    }
    this->usedMemeryList = nullptr;

    ui->check1->setChecked(true);
}


void Memory::dye(struct usedMemeryBlock * block,int flag){
    int j;
    int i;
    for (int k=0;k<block->memeryBlockSize;k++){
//        qDebug()<<text;
        i = block->pageList[k][1]/20;
        j = block->pageList[k][1]%20;
        ui->memeryTable->item(i,j)->setFont(QFont("song", 10));
        if(flag==0){
            QString text;
            text = block->pid+"|-1";
            ui->memeryTable->item(i,j)->setText(text);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(color[block->ranking%10][0],color[block->ranking%10][1],color[block->ranking%10][2])));
        }
        else if(flag==1){
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(200,200,200)));
            ui->memeryTable->item(i,j)->setText("");
        }
        else{
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(color[block->ranking%10][0],color[block->ranking%10][1],color[block->ranking%10][2])));
        }
    }
}
//请求内存块 首次适应算法

//bool Memory::requestMemery1(int pageFrame,QString pid){
////    qDebug()<<"1111";
//    struct freeMemeryBlock * tempBlock1;
//    struct freeMemeryBlock * tempBlock2;
//    tempBlock2 = nullptr;
//    if(this->freeMemeryList == nullptr) return false;
//    tempBlock1 = this->freeMemeryList;
//    this->BubbleSort(tempBlock1);//空闲区排序
//    int flag = 0;
//    while(tempBlock1!=nullptr){
//        if(tempBlock1->memeryBlockSize>=pageFrame){

//            struct usedMemeryBlock * block = new struct usedMemeryBlock();
//            block->memeryBlockSize = pageFrame;

//            for(int i=0;i<5;i++){

//                block->pageList[i][0] = -1;

//            }
//            for(int i=0;i<50;i++){
//                block->requestPageList[i] = -1;
//            }

////            block->endIndex = tempBlock1->startIndex + pageFrame - 1;
////            block->startIndex = tempBlock1->startIndex;
//            block->pid = pid;
//            block->nextBlock = this->usedMemeryList;
//            this->usedMemeryList = block;
//            this->dye(block,0);//对已分配的内存进行染色
//            qDebug()<<tempBlock1->memeryBlockSize<<tempBlock1->startIndex;
//            if(tempBlock1->memeryBlockSize!=pageFrame){
//               tempBlock1->memeryBlockSize = tempBlock1->memeryBlockSize-pageFrame;
//               tempBlock1->startIndex = tempBlock1->startIndex + pageFrame;
//            }
//            else {
//                if(tempBlock2==nullptr){
//                    this->freeMemeryList = tempBlock1->nextBlock;
//                    free(tempBlock1);
//                }
//                else{
//                    tempBlock2->nextBlock = tempBlock1->nextBlock;
//                    free(tempBlock1);
//                }
//            }
//            return true;
//        }
//        tempBlock1 = tempBlock1->nextBlock;
//        if(flag==0){
//          tempBlock2 = this->freeMemeryList;
//        }
//        else {
//            tempBlock2 = tempBlock2->nextBlock;
//            flag = 1;
//        }
//    }

//    return false;
//}

//请求内存块 分页式
bool Memory::requestMemery(int pageFrame,QString pid){
    int count = 0;
    struct usedMemeryBlock * block = new struct usedMemeryBlock();
    block->memeryBlockSize = pageFrame;
    block->pid = pid;
    block->nextBlock = this->usedMemeryList;
    block->ranking = this->proCount;
    this->usedMemeryList = block;
    int i;
    for(i=0;i<50;i++){
        block->requestPageList[i] = -1;
    }
    for(i=0;i<pageFrame;i++){
         block->pageList[i][0] = -1;
    }
    for(int i=0;i<400;i++){
        if(this->isUsed[i]==0){
            block->pageList[count][1] = i;
            this->isUsed[i] = 1;
            count++;
            if(count==pageFrame) break;
        }
    }
    if(i==400) return false;
    this->dye(block,0);//对已分配的内存进行染色
    this->proCount++;
    w->diskTab->createVirtualMemoryBlock(pid,block->ranking);
    return true;
}
//链表排序
void Memory:: BubbleSort(freeMemeryBlock * &L)
{
    int i ,count = 0, num;//count记录链表结点的个数，num进行内层循环，
    freeMemeryBlock *p, *q, *tail;//创建三个指针，进行冒泡排序
    p = L;
    struct freeMemeryBlock * newBlock = new struct freeMemeryBlock();
    newBlock->memeryBlockSize = 20*20;
    newBlock->endIndex = 399;
    newBlock->startIndex = 0;
    newBlock->nextBlock = p;
    L = newBlock;
    p = L;
    while(p->nextBlock!= nullptr)//计算出结点的个数
    {
        count++;//注释①
        p = p->nextBlock;
//        qDebug()<<count<<"+++";
    }
//    qDebug()<<count;
    for(i = 0; i < count - 1; i++)//外层循环，跟数组冒泡排序一样
    {
        num = count - i - 1;//记录内层循环需要的次数，跟数组冒泡排序一样，
        q = L->nextBlock;//令q指向第一个结点
        p = q->nextBlock;//令p指向后一个结点
        tail = L;//让tail始终指向q前一个结点，方便交换，也方便与进行下一步操作
        while(num--)//内层循环 次数跟数组冒泡排序一样
        {
            if(q->startIndex > p->startIndex)//如果该结点的值大于后一个结点，则交换
            {
//                qDebug()<<num<<"===";
                q->nextBlock = p->nextBlock;
                p->nextBlock = q;
                tail->nextBlock = p;
            }
            tail = tail->nextBlock;//注释②
            q = tail->nextBlock;//注释②
            p = q->nextBlock;//注释②
         }
    }
    free(newBlock);
    //
    p = L->nextBlock;
    L = L->nextBlock;

//    while(p!= nullptr)//计算出结点的个数
//    {
//        qDebug()<<p->memeryBlockSize<<"----"<<p->startIndex;
//        p = p->nextBlock;

//    }
}
//合并空闲内存
void Memory::mergeFreeMemery(){
    freeMemeryBlock *p,*q;
    this->BubbleSort(this->freeMemeryList);
    p = this->freeMemeryList;
    if(p == nullptr) return;
    while(p->nextBlock!=nullptr){
        if(p->endIndex == p->nextBlock->startIndex-1){
            p->memeryBlockSize = p->memeryBlockSize + p->nextBlock->memeryBlockSize;
            p->endIndex = p->nextBlock->endIndex;
            q = p->nextBlock;
            p->nextBlock = p->nextBlock->nextBlock;
            free(q);//释放内存空间
            continue;
        }
        p = p->nextBlock;
    }
}
//释放内存
void Memory::freeMemery(QString pid){
    struct usedMemeryBlock * block;
    struct usedMemeryBlock * block1;
    block = this->usedMemeryList;
    w->diskTab->freeMemory(pid);
    if(block->pid == pid){
        this->usedMemeryList = block->nextBlock;
        for(int i =0;i<block->memeryBlockSize;i++){
            this->isUsed[block->pageList[i][1]] = 0;
        }
        this->dye(block,1);
        free(block);

    }
    else{
        while(block->nextBlock!=nullptr && block->nextBlock->pid != pid){
            qDebug()<<block->nextBlock->pid<<"44444";
            block = block->nextBlock;
        }
        if(block->nextBlock==nullptr){
            qDebug()<<"pid找不到"<<pid;
        }
        else{

            for(int i =0;i<block->nextBlock->memeryBlockSize;i++){
                this->isUsed[block->nextBlock->pageList[i][1]] = 0;
            }
            this->dye(block->nextBlock,1);
            block1 = block->nextBlock;
            block->nextBlock = block->nextBlock->nextBlock;
            free(block1);
        }
    }
}

//先进先出置换算法
void Memory::replacePageByFIFO(QString pid,int page){

    struct usedMemeryBlock * block;
    block = this->usedMemeryList;
    while(block != nullptr && block->pid != pid){
        block = block->nextBlock;
    }
    if(block == nullptr) return;
    block->requestPageList[block->requestPageCount] = page;
    block->requestPageCount++;
    int i,j;

    for(int k=0;k<block->memeryBlockSize;k++){
        //进入页在内存中

        i = block->pageList[k][1]/20;
        j = block->pageList[k][1]%20;
        if(block->pageList[k][0] == page){
            this->dye(block,2);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(16,126,239)));
            return;
        }
    }
    //内存空间未满
    if(block->blockSize<block->memeryBlockSize){
        block->FIFOPageQueue.enqueue(page);
        block->blockSize++;
        block->pageList[block->blockSize-1][0] = page;
        i = block->pageList[block->blockSize-1][1]/20;
        j = block->pageList[block->blockSize-1][1]%20;
        this->dye(block,2);
        ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(i,j)->setText(QString::number(page));
        w->diskTab->replacePage(pid,page);
    }
    else{
        int OutPage = block->FIFOPageQueue.dequeue();
        block->FIFOPageQueue.enqueue(page);
        //取出需要置换的位置
        int index;
        for(index=0;index<block->memeryBlockSize;index++){
            if(block->pageList[index][0] == OutPage){
                break;
            }
        }
        w->diskTab->replacePage(pid,page);
        int xindex = block->pageList[index][1]/20;
        int yindex = block->pageList[index][1]%20;
        this->dye(block,2);
        block->pageList[index][0] = page;
        ui->memeryTable->item(xindex,yindex)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(xindex,yindex)->setText(QString::number(page));
    }
}
//页面置换算法 最久未使用算法
void Memory::replacePageByLRU(QString pid,int page){
    //找到该进程的内存
    struct usedMemeryBlock * block;
    block = this->usedMemeryList;
    while(block != nullptr && block->pid != pid){
        block = block->nextBlock;
    }
    if(block == nullptr) return;
    block->requestPageList[block->requestPageCount] = page;
    block->requestPageCount++;
//    qDebug()<<"----"<<page<<pid;
    //初始化
    int j;
    int i;
    for(int k=0;k<block->memeryBlockSize;k++){
        //进入页在内存中

        i = block->pageList[k][1]/20;
        j = block->pageList[k][1]%20;
        if(block->pageList[k][0] == page){
            this->dye(block,2);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(16,126,239)));
            return;
        }
    }

    //内存空间未满
    if(block->blockSize<block->memeryBlockSize){
        block->blockSize++;
        block->pageList[block->blockSize-1][0] = page;
        i = block->pageList[block->blockSize-1][1]/20;
        j = block->pageList[block->blockSize-1][1]%20;
        this->dye(block,2);
        ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(i,j)->setText(QString::number(page));
        w->diskTab->replacePage(pid,page);
    }
    else{
        //寻找替换的位置
        int k;
        int postion[block->memeryBlockSize];
        int count = 0;
        for(k = 0;k<block->memeryBlockSize;k++){
            postion[k] = -1;
        }
        //对前面的页进行扫描
        for(k = block->requestPageCount-1;k>=0;k--){
            int z;
            for(z=0;z<block->memeryBlockSize;z++){
                if(block->requestPageList[k] == block->pageList[z][0]){
                    if(postion[z]!=0)
                    {
                        postion[z] = 0;
                        count++;
                    }
                }
            }
            if(count == block->memeryBlockSize-1){
                break;
            }
        }
        //取出需要置换的位置
        int index;
        for(index=0;index<block->memeryBlockSize;index++){
            if(postion[index]==-1){
                break;
            }
        }
        //置换
        w->diskTab->replacePage(pid,page);
        int xindex = block->pageList[index][1]/20;
        int yindex = block->pageList[index][1]%20;
        this->dye(block,2);
        block->pageList[index][0] = page;
        ui->memeryTable->item(xindex,yindex)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(xindex,yindex)->setText(QString::number(page));

    }
}

void Memory::replacePage(QString pid,int page){
    if (ui->check1->isChecked()) {
        // timer->start(1000); //启动定时器，运行完一个进程
        this->replacePageByLRU(pid,page);
    }else if(ui->check2->isChecked()){
        this->replacePageByFIFO(pid,page);
    }else{
        QMessageBox::warning(this,"错误提示",tr("请选择置换算法算法"));
        return ;
    }
}

void Memory::paintEvent(QPaintEvent *){
    QPainter painter(this); //指定绘图设备


}
