#include "memory.h"
#include "ui_memory.h"
#include <QPushButton>
#include <QMenuBar>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
Memory::Memory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);
//    QMessageBox::information(this,"info","信息");
    int columnCount = 20;
    int rowCount = 20;
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
    this->requestMemery(5,2);
    this->requestMemery(3,1);
    QTimer * timer = new QTimer(this);
    timer->start(500);
    Memory * that = this;
    connect(timer,&QTimer::timeout,[=](){
        that->freeMemery(2);
        timer->stop();
    });
}

Memory::~Memory()
{
    delete ui;
}
//初始化内存块的函数；
void Memory::initMemery(){
    struct freeMemeryBlock * newBlock = new struct freeMemeryBlock();
    newBlock->memeryBlockSize = 20*20;
    newBlock->endIndex = 399;
    newBlock->startIndex = 0;
    newBlock->nextBlock = nullptr;
    this->freeMemeryList = newBlock;
    this->usedMemeryList = nullptr;
}
void Memory::dye(struct usedMemeryBlock * block,int flag){
    int j = block->startIndex%20;
    int i = block->startIndex/20;
    for (int k=0;k<block->memeryBlockSize;k++){
        QString text;
        text = QString::number(block->pid)+"|-1";
//        qDebug()<<text;
        ui->memeryTable->item(i,j)->setText(text);
        ui->memeryTable->item(i,j)->setFont(QFont("song", 10));
        if(flag==0)
        ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(237,19,80)));
        else {
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(200,200,200)));
            ui->memeryTable->item(i,j)->setText("");
        }
        j++;
        if(j==20){
            i++;
            j = 0;
        }
    }
}
//请求内存块 最佳适配算法
bool Memory::requestMemery(int pageFrame,int pid){
    struct freeMemeryBlock * tempBlock1;
    if(this->freeMemeryList == nullptr) return false;
    tempBlock1 = this->freeMemeryList;
    this->BubbleSort(tempBlock1);//空闲区排序

    while(tempBlock1!=nullptr){

        if(tempBlock1->memeryBlockSize>pageFrame){

            struct usedMemeryBlock * block = new struct usedMemeryBlock();
            block->memeryBlockSize = pageFrame;

            for(int i=0;i<5;i++){

                block->pageList[i] = -1;
            }
            block->endIndex = tempBlock1->startIndex + pageFrame - 1;
            block->startIndex = tempBlock1->startIndex;
            block->pid = pid;
            if(this->usedMemeryList!=nullptr)
                block->nextBlock = this->usedMemeryList;
            this->usedMemeryList = block;
            qDebug()<<"1111";
            this->dye(block,0);//对已分配的内存进行染色

            if(tempBlock1->memeryBlockSize!=pageFrame){
               tempBlock1->memeryBlockSize = tempBlock1->memeryBlockSize-pageFrame;
               tempBlock1->startIndex = tempBlock1->startIndex + pageFrame;
            }
            else {
                tempBlock1 = nullptr;
            }
            return true;
        }
        tempBlock1 = tempBlock1->nextBlock;
    }
    return false;
}
//链表排序
void Memory:: BubbleSort(freeMemeryBlock * &L)
{
    int i ,count = 0, num;//count记录链表结点的个数，num进行内层循环，
    freeMemeryBlock *p, *q, *tail;//创建三个指针，进行冒泡排序
    p = L;
    while(p!= nullptr)//计算出结点的个数
    {
        count++;//注释①
        p = p->nextBlock;
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
                q->nextBlock = p->nextBlock;
                p->nextBlock = q;
                tail->nextBlock = p;
            }
            tail = tail->nextBlock;//注释②
            q = tail->nextBlock;//注释②
            p = q->nextBlock;//注释②
         }
    }
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
void Memory::freeMemery(int pid){
    struct usedMemeryBlock * block;
    struct usedMemeryBlock * block1;
    block = this->usedMemeryList;
//    block1 = this->usedMemeryList;
//    while(block1!=nullptr){
//        qDebug()<<"eeee";
//        qDebug()<<block1->pid;
//        block1 = block1->nextBlock;
//    }
    if(block->pid == pid){
        this->usedMemeryList = block->nextBlock;
        struct freeMemeryBlock * newBlock = new struct freeMemeryBlock();
        newBlock->memeryBlockSize = block->memeryBlockSize;
        newBlock->endIndex = block->endIndex;
        newBlock->startIndex = block->startIndex;
        newBlock->nextBlock = this->freeMemeryList;
        this->freeMemeryList = newBlock;
        this->dye(block,1);
        free(block);
    }
    else{
        while(block->nextBlock!=nullptr && block->nextBlock->pid != pid){
            qDebug()<<block->pid;
            block = block->nextBlock;
        }
        if(block->nextBlock==nullptr){
            qDebug()<<"pid找不到";
        }
        else{

            struct freeMemeryBlock * newBlock = new struct freeMemeryBlock();
            newBlock->memeryBlockSize = block->memeryBlockSize;
            newBlock->endIndex = block->endIndex;
            newBlock->startIndex = block->startIndex;
            newBlock->nextBlock = this->freeMemeryList;
            this->freeMemeryList = newBlock;
            this->dye(block->nextBlock,1);
            block->nextBlock = block->nextBlock->nextBlock;
            free(block->nextBlock);
        }
    }
}

void Memory::paintEvent(QPaintEvent *){
    QPainter painter(this); //指定绘图设备


}
