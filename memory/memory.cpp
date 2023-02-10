#include "memory.h"
#include "ui_memory.h"
#include <QPushButton>
#include <QMenuBar>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

#include <QDateTime>
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

    // 测试函数的代码

//    this->requestMemery(5,"22");
//    this->requestMemery(3,"21");
//    QTimer * timer = new QTimer(this);
//    QTimer * timer2 = new QTimer(this);
//    timer->start(1000);
//    Memory * that = this;
//    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//    connect(timer,&QTimer::timeout,[=](){
//        that->freeMemery("22");
//        timer->stop();
//    });
//    timer2->start(1000);
//    connect(timer2,&QTimer::timeout,[=](){
//        int ran = rand() % 10;
//        that->replacePageByLRU("21",ran);
//    });
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
//        qDebug()<<text;
        ui->memeryTable->item(i,j)->setFont(QFont("song", 10));
        if(flag==0){
            QString text;
            text = block->pid+"|-1";
            ui->memeryTable->item(i,j)->setText(text);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(237,19,80)));
        }
        else if(flag==1){
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(200,200,200)));
            ui->memeryTable->item(i,j)->setText("");
        }
        else{
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(237,19,80)));
        }

        j++;
        if(j==20){
            i++;
            j = 0;
        }
    }
}
//请求内存块 最佳适配算法
bool Memory::requestMemery(int pageFrame,QString pid){
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
            for(int i=0;i<50;i++){
                block->requestPageList[i] = -1;
            }

            block->endIndex = tempBlock1->startIndex + pageFrame - 1;
            block->startIndex = tempBlock1->startIndex;
            block->pid = pid;
            if(this->usedMemeryList!=nullptr)
                block->nextBlock = this->usedMemeryList;
            this->usedMemeryList = block;

            //qDebug()<<"1111";

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
void Memory::freeMemery(QString pid){
    struct usedMemeryBlock * block;
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
    //初始化
    int j = block->startIndex%20;
    int i = block->startIndex/20;
    for(int k=0;k<block->memeryBlockSize;k++){
        //进入页在内存中
        if(block->pageList[k] == page){
            this->dye(block,2);
            ui->memeryTable->item(i,j)->setBackground(QBrush(QColor(16,126,239)));
            return;
        }
        j++;
        if(j==20){
            i++;
            j = 0;
        }
    }
    //内存空间未满
    if(block->requestPageCount<=block->memeryBlockSize){
        block->pageList[block->requestPageCount-1] = page;
        int index = (block->startIndex+block->requestPageCount-1)/20;
        int jindex = (block->startIndex+block->requestPageCount-1)%20;
        this->dye(block,2);
        ui->memeryTable->item(index,jindex)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(index,jindex)->setText(QString::number(page));
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
                if(block->requestPageList[k] == block->pageList[z]){
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
        int xindex = (block->startIndex+index)/20;
        int yindex = (block->startIndex+index)%20;
        this->dye(block,2);
        block->pageList[index] = page;
        ui->memeryTable->item(xindex,yindex)->setBackground(QBrush(QColor(16,126,239)));
        QString text;
//        text = "" + page;
        ui->memeryTable->item(xindex,yindex)->setText(QString::number(page));
    }
}


void Memory::paintEvent(QPaintEvent *){
    QPainter painter(this); //指定绘图设备


}
