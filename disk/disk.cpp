#include "disk.h"
#include "ui_disk.h"


int color1[10][3] = {
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
        ui->virtualMemory->item(i,k)->setBackground(QBrush(QColor(color1[ranking%10][0],color1[ranking%10][1],color1[ranking%10][2])));
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
        this->isUsed[block->startIndex] = 0;
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


/*****************************************************  磁盘调度  **************************************************************/
int * Disk::disk_sheduling(QVector<int> seq){
    int n = seq.size();
    int start = 1;
    int xb;
    const int INF = 0x3f3f3f3f;
    double avgfind;
    typedef pair<int, int> p;
    vector<p> v;

    sort(seq.begin(), seq.end());

   if(seq[0] > start) {
       v.push_back({seq[0], abs(seq[0] - start)});
       avgfind += abs(seq[0] - start);
       start = seq[0];

       xb = 1;

       for(int i = xb; i < seq.size(); i ++) {
           v.push_back({seq[i], abs(seq[i] - start)});
           avgfind += abs(seq[i] - start);
           start = seq[i];
       }
   }
   else if(seq[n - 1] < start) {
       v.push_back({seq[n - 1], abs(start - seq[n - 1])});
       avgfind += abs(start - seq[n - 1]);
       xb = n - 2;
       start = seq[n - 1];

       for(int i = xb; i >= 0; i --) {
           v.push_back({seq[i], abs(seq[i] - start)});
           avgfind += abs(seq[i] - start);
           start = seq[i];
       }

   } else {
       for(int i = 0; i < seq.size() - 1; i ++) {
       if(seq[i + 1] > start && seq[i] < start) {
           v.push_back({seq[i + 1], abs(seq[i + 1] - start)});
           avgfind += abs(seq[i + 1] - start);
           start = seq[i + 1];
           xb = (i + 2 > seq.size()) ? seq.size() : (i + 2);
           break;
       }
   }
        for(int i = xb; i < seq.size(); i ++) {
           v.push_back({seq[i], abs(seq[i] - start)});
           avgfind += abs(seq[i] - start);
           start = seq[i];
       }
       for(int i = xb - 2; i >= 0; i --) {
           v.push_back({seq[i], abs(seq[i] - start)});
           avgfind += abs(seq[i] - start);
           start = seq[i];
       }
   }

//       cout << "磁道号\t移动距离" << "\n";
//       for(int i = 0; i < v.size(); i ++) {
//           cout << v[i].first << "\t" << v[i].second << "\n";
//       }
//       cout << "总长度：" <<endl;
//       printf("%.2lf\n", avgfind);
//       cout << "平均寻道长度：" <<endl;
//       printf("%.2lf", avgfind / temp);
   int track[100];
   for(int i = 0; i < v.size(); i ++){
       track[i] = v[i].first;
   }
   return track;
}

void Disk::ShowDiskTrack(int track_num){

}

Disk::~Disk()
{
    delete ui;
}
