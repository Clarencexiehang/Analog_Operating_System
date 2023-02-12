#include "disk.h"
#include "ui_disk.h"
int color1[5][3] = {
    {33,222,28},
    {188,133,240},
    {237,14,111},
    {240,20,138},
    {24,237,14}
};
Disk::Disk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disk)
{
    ui->setupUi(this);
    this->initvirtualMemory();
}
void Disk::createVirtualMemoryBlock(QString pid,int ranking){
    int i;
    for(i=0;i<20;i++){
        if(!this->isUsed[i]) break;
    }
    this->isUsed[i] = 1;
    struct virtualMemory *block = new struct virtualMemory();
    block->pid = pid;
    block->startIndex = i;
    block->ranking = ranking;
    block->nextBlock = this->virtualMemoryBlock;
    this->virtualMemoryBlock = block;
    for (int k=0;k<20;k++){
        ui->virtualMemory->item(i,k)->setBackground(QBrush(QColor(color1[ranking][0],color1[ranking][1],color1[ranking][2])));
    }
}
void Disk::initvirtualMemory(){
    int i;
    int columnCount = 20;
    int rowCount = 20;
    ui->virtualMemory->setColumnCount(columnCount);
    ui->virtualMemory->setRowCount(rowCount);
//    ui->processtable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->virtualMemory->setAlternatingRowColors(true);
    ui->virtualMemory->verticalHeader()->setDefaultSectionSize(1);
    ui->virtualMemory->horizontalHeader()->setDefaultSectionSize(1);
    ui->virtualMemory->verticalHeader()->setVisible(false);
    ui->virtualMemory->horizontalHeader()->setVisible(false);
    ui->virtualMemory->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选择每行且单元不可编辑
//    ui->memeryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->virtualMemory->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->virtualMemory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i=0;i<columnCount;i++){
        for(int j=0;j<rowCount;j++){
            QTableWidgetItem  *tableitem = new QTableWidgetItem(QString::number(j));
            tableitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->virtualMemory->setItem(i,j,tableitem);
            ui->virtualMemory->item(i,j)->setBackground(QBrush(QColor(200,200,200)));

        }
    }
    for(i = 0;i<20;i++){
        this->isUsed[i] = 0;
    }
}
void Disk::replacePage(QString pid,int page){
    struct virtualMemory *block = this->virtualMemoryBlock;
    while (block != nullptr && block->pid!=pid) {
        block=block->nextBlock;
    }
    if(block == nullptr) return;
    for (int k=0;k<20;k++){
        ui->virtualMemory->item(block->startIndex,k)->setBackground(QBrush(QColor(color1[block->ranking][0],color1[block->ranking][1],color1[block->ranking][2])));
    }
    ui->virtualMemory->item(block->startIndex,page)->setBackground(QBrush(QColor(16,126,239)));
}
void Disk::freeMemory(QString pid){
    struct virtualMemory *block = this->virtualMemoryBlock;
    if(block->pid==pid){
        for (int k=0;k<20;k++){
            ui->virtualMemory->item(block->startIndex,k)->setBackground(QBrush(QColor(200,200,200)));
        }
        this->virtualMemoryBlock = block->nextBlock;
        free(block);
        return;
    }
    while (block->nextBlock != nullptr && block->nextBlock->pid!=pid) {
        block=block->nextBlock;
    }
    if(block->nextBlock == nullptr) return;
    for (int k=0;k<20;k++){
        ui->virtualMemory->item(block->nextBlock->startIndex,k)->setBackground(QBrush(QColor(200,200,200)));
    }
    this->isUsed[block->nextBlock->startIndex] = 0;
    block->nextBlock = block->nextBlock->nextBlock;

}
Disk::~Disk()
{
    delete ui;
}
