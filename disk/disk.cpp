#include "disk.h"
#include "ui_disk.h"
#include"mainwindow.h"

extern MainWindow *w;

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
    //初始化磁盘块表格 **********************************
    //行列
    ui->block_tab->horizontalHeader()->setVisible(false);
    ui->block_tab->setRowCount(8);
    ui->block_tab->setColumnCount(15);
    for(int i=0;i<15;i++){
        ui->block_tab->setColumnWidth(i,25);
    }
    for(int i=0;i<8;i++){
        ui->block_tab->setRowHeight(i,25);
    }
    //颜色
    for(int i=0;i<10;i++){
        for(int j=0;j<20;j++){
            QTableWidgetItem *t=new QTableWidgetItem;
            t->setBackground(QBrush(Qt::gray));
            ui->block_tab->setItem(i,j,t);
        }
    }
    //初始化磁盘块**********************************
    //块数据
    int bid=0;//块数
    for(int i=0;i<10;i++){
        for(int j=0;j<20;j++){
            Block *newBlock=new Block;
            newBlock->blockId=bid;
            newBlock->fileId=-1;
            newBlock->filename=nullptr;
            newBlock->size=0;

            bid++;
            diskBlock.append(newBlock);
        }
    }
    // 占用 块
    this->setBlockBeFull(0,1);
    this->setBlockBeFull(7,11);
    this->setBlockBeFull(40,43);
    this->setBlockBeFull(80,90);

    //磁道  50 个磁道
    for (int i=0;i<diskBlock.size();i++) {
        int r=i/15+1;
        diskBlock[i]->cyId=r;
    }
    //初始化空闲空间表数据**********************************
    this->flushFreeBlockTab();

    //显示空闲空间列表窗口**********************************
    freeblobktab=new freeBlockTable;
    freeblobktab->show();
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    for (int r=0;r<freeblockTab.size();r++) {
        qDebug()<<"表窗口添加 ： "<<freeblockTab[r].blockId<<" "<<freeblockTab[r].con_num;
        QTableWidgetItem *td=new QTableWidgetItem;
        td->setText(QString::number(freeblockTab[r].blockId));
        ftb->setItem(r,0,td);

        QTableWidgetItem *td1=new QTableWidgetItem;
        td1->setText(QString::number(freeblockTab[r].con_num));
        ftb->setItem(r,1,td1);
    }



    //+++++++++++++++++++++++++++++++++++ 调试
    this->showBlockInfo();
    this->showFreeData();
    //+++++++++++++++++++++++++++++++++++
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


/******************* 金平  ********************************/
void Disk::fileExband(QString policy, QString filename, QString exbandType, QString exbanSize)
{
    if(exbandType=="扩展"){
        qDebug()<<"扩展 ："<<exbanSize;
        this->fileToMore(policy,filename,exbanSize);
    }else{
        qDebug()<<"缩小 ："<<exbanSize;
        this->fileToFew(policy,filename,exbanSize);
    }

    qDebug()<<" PCB __________________ ";
    QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
    for (int i=0;i<pcb.size();i++) {
        qDebug()<<" name:"<<pcb[i]->name<<"  size:"<<pcb[i]->blockNum;
    }

    this->showBlockInfo();
    this->showFreeData();
}

void Disk::fileToMore(QString policy, QString filename, QString exbanSize)
{
    int size=exbanSize.toInt();
    if(size!=0){
        qDebug()<<" "<<policy<<" 文件 "<<filename<<" 扩展 "<<size<<" KB";
        //
        int blockSize=10;//块大小
        int blockNum=3;//每个索引块包含磁盘块数 30k
        //获取空闲空间列表表格控件
        QTableWidget *ftb=freeblobktab->getFreeBlockTable();
        if(policy=="连续分配"){ // size (KB)
            if(freeblockTab.size()==0){
                //剩余空间不够
                QMessageBox::critical(this,"分配失败",tr("空闲表无可用空间记录 ！"));
            }else{
                //计算文件需要多少块
                int ms=size/blockSize;
                int my=size%blockSize;
                int needNum=my>0?(ms+1):ms;
                qDebug()<<" 需要 "<<needNum<<" 个磁盘块";
                bool hasConBlock=false;//是否有足够连续块
                //找到最小连续块
                int minConindex=-1;//最小连续块在空闲表中所在位置
                int conMinPos=-1;//连续块数最小起始块号
                int minConNum=1000000;//最小块数
                qDebug()<<"初始空闲空间表 ————————————————————";
                for(int i=0;i<freeblockTab.size();i++){
                    if(freeblockTab[i].con_num>=needNum){
                        //找到第一可足够连续块
                        qDebug()<<" 找到  "<<i<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
                        hasConBlock=true;
                        minConindex=i;
                        conMinPos=freeblockTab[i].blockId;
                        minConNum=freeblockTab[i].con_num;
                        break;
                    }
                }
                qDebug()<<"找到最小连续块记录  index:"<<minConindex<<" blockId:"<<conMinPos<<" num:"<<minConNum;
                //目录内找到起始块
                int id;
                int confBlockId;
                for (int i=0;i<confiledir.size();i++) {
                    if(confiledir[i].filename==filename){
                        id=confiledir[i].fileId;
                        confBlockId=confiledir[i].first_block;
                    }
                }
                //文件块表内找到最后一个块记录 (下一扩展块值 -2)
                int endIndex=0;
                while (confBlockId!=-2) {
                    qDebug()<<"寻找 块 :"<<confBlockId<<" 行号 ";
                    for (int i=0;i<fileBlocks.size();i++) {
                        if(fileBlocks[i].firstBlockId==confBlockId){
                            qDebug()<<" 找到结尾块 :"<<confBlockId<<" 行号 "<<i;
                            endIndex=i;
                            break;
                        }
                    }
                    confBlockId=fileBlocks[endIndex].nextBlockId;
                }
                //上一个文件块结尾修改记录
                fileBlocks[endIndex].nextBlockId=conMinPos;
                //
                //修改文件大小
                QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
                for (int i=0;i<pcb.size();i++) {
                    if(pcb[i]->name==filename){
                        pcb[i]->blockNum=pcb[i]->blockNum+needNum;
                    }
                }
                //文件块表添加记录
                FileBlock fb;
                fb.firstBlockId=conMinPos;
                fb.blockNum=needNum;
                fb.nextBlockId=-2;//  ***************** -2 表示文件结束
                fileBlocks.append(fb);
                qDebug()<<"新文件块表 : ++++++++++++++++++++++++++++";
                for (int i=0;i<fileBlocks.size();i++) {
                    qDebug()<<" f:"<<fileBlocks[i].firstBlockId<<" n:"<<fileBlocks[i].blockNum<<" ne:"<<fileBlocks[i].nextBlockId;
                }
                if(hasConBlock==true){
                    qDebug()<<"开始连续分配 ："<<policy;
                    //开始分配
                    for(int i=conMinPos;i<conMinPos+needNum;i++){
                        qDebug()<<" 分配空闲块 ："<<i;
                        //设置磁盘块的文件 id
                        diskBlock[i]->fileId=id;
                        //修改块颜色
                        int rr=i/(ui->block_tab->columnCount());
                        int cc=i%(ui->block_tab->columnCount());
                        qDebug()<<"修改块颜色 ："<<i<<"  r: "<<rr<<" c:"<<cc;
                        QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        tt->setText(QString::number(diskBlock[i]->fileId));

                    }
                    if(minConNum==needNum){
                        qDebug()<<"空闲表删除记录 "<<minConindex<<freeblockTab[minConindex].blockId<<freeblockTab[minConindex].con_num;
                        freeblockTab.erase(freeblockTab.begin()+minConindex);
                        ftb->removeRow(minConindex);
                    }else{
                        //空闲表修改记录
                        qDebug()<<"空闲表修改记录 "<<minConindex<<freeblockTab[minConindex].blockId<<freeblockTab[minConindex].con_num;
                        freeblockTab[minConindex].blockId=conMinPos+needNum;
                        freeblockTab[minConindex].con_num=minConNum-needNum;

                        qDebug()<<"空闲表修改记录为 ："<<conMinPos+needNum<<" "<<minConNum-needNum;
                        QTableWidgetItem *td0=ftb->item(minConindex,0);
                        td0->setText(QString::number(conMinPos+needNum));
                        QTableWidgetItem *td1=ftb->item(minConindex,1);
                        td1->setText(QString::number(minConNum-needNum));
                    }
                    qDebug()<<"更新后空闲空间表 ————————————————————";
                    for(int i=0;i<freeblockTab.size();i++){
                        qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
                    }
                }else{
                    //最小连续块不够
                    QMessageBox::critical(this,"分配失败",tr("空闲表最小连续块不够 ！"));
                }
            }

    //******************************** 显式链接 ****************************************
        }else if(policy=="显式链接"){ // size (KB)
            if(freeblockTab.size()==0){
                //剩余空间不够
                QMessageBox::critical(this,"分配失败",tr("空闲表无可用空间记录 ！"));
            }else{
                qDebug()<<"空闲空间表 ————————————————————";
                int freSpace=0;//剩余总空间
                for(int i=0;i<freeblockTab.size();i++){
                    qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
                    freSpace=freSpace+blockSize*freeblockTab[i].con_num;
                }
                //判断剩余的空间是否足够分配
                if(freSpace<size){
                    //剩余空间不够
                    QMessageBox::critical(this,"分配失败",tr("剩余空间不足 ！"));
                }
                else{
                    qDebug()<<"开始显式链接分配 FAT ："<<policy;
                    //修改文件大小
                    int ms=size/blockSize;
                    int my=size%blockSize;
                    int needNum=my>0?(ms+1):ms;
                    QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
                    for (int i=0;i<pcb.size();i++) {
                        if(pcb[i]->name==filename){
                            pcb[i]->blockNum=pcb[i]->blockNum+needNum;
                        }
                    }
                    //目录中找到首块号
                    int id;
                    int lafirstId;
                    for (int i=0;i<fatDir.size();i++) {
                        if(fatDir[i].filename==filename){
                            id=fatDir[i].fileId;
                            lafirstId=fatDir[i].first_block;
                            break;
                        }
                    }
                    //FAT 表中寻找最后一块所在行
                    int lastRowIndex=lafirstId;
                    int log=lastRowIndex;
                    while (lastRowIndex!=-2) {
                        qDebug()<<" FAT 找到 r: "<<lastRowIndex<<" i:"<<Fat[lastRowIndex].blockId<<" v:"<<Fat[lastRowIndex].value;
                        lastRowIndex=Fat[lastRowIndex].value;
                        if(lastRowIndex!=-2){
                            log=lastRowIndex;
                        }
                    }

                    qDebug()<<"找到 FAT 末尾记录 log："<<log;

                    int firstId=-1;
                    int hasgetSpace=0;
                    int nextId=-2;

                    bool isFirst=true;//当前寻找是否第一空闲块
                    int ffirst;//第一个块号
                    while (hasgetSpace<size) {
                        //寻找第一个空闲块
                        for (int i=0;i<Fat.size();i++){
                            if(Fat[i].value==-1){
                                nextId=Fat[i].blockId;//找到第一空闲块 i

                                if(isFirst){
                                    qDebug()<<"第一连续块 找到首块 ：_____ "<<i;
                                    //当前寻找的为第一空闲块
                                    ffirst=i;
                                    //将上一个文件结尾链接
                                    Fat[log].value=ffirst;
                                    isFirst=false;
                                }
                                break;
                            }
                        }
                        qDebug()<<"继续分配 块 : "<<nextId;
                        //在空闲表中移除块 nextId;
                        this->disrtSpaceInFreeTable(nextId);
                        //修改磁盘块文件 id
                        diskBlock[nextId]->fileId=id;
                        //把下一个空闲块加入
                        for (int i=0;i<Fat.size();i++){
                            if(Fat[i].blockId==firstId){
                                Fat[i].value=nextId;
                                break;
                            }
                        }
                        //文件尾部空闲块 FAT v 置 -2
                        for (int i=0;i<Fat.size();i++){
                            if(Fat[i].blockId==nextId){
                                Fat[i].value=-2;
                                break;
                            }
                        }
                        int rcl=nextId/(ui->block_tab->columnCount());
                        int ccl=nextId%(ui->block_tab->columnCount());
                        QTableWidgetItem *tb=ui->block_tab->item(rcl,ccl);
                        tb->setText(QString::number(id));
                        tb->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        hasgetSpace=hasgetSpace+10;
                        qDebug()<<"已分配 : "<<hasgetSpace;

                        firstId=nextId;
                        nextId=-2;
                    }
                    qDebug()<<"------------- 分配完成 !--------------";
                    //查看 FAT
                    for(int i=0;i<Fat.size();i++){
                        qDebug()<<"FAT 记录 ："<<i<<" v: "<<Fat[i].value;
                    }

                }

            }

    //*********************************** 索引分配 ****************************
        }else{ // size (KB)
            int size=exbanSize.toInt();
            qDebug()<<"空闲空间表 ————————————————————";
            int freSpace=0;//剩余总空间
            for(int i=0;i<freeblockTab.size();i++){
                qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
                freSpace=freSpace+blockSize*freeblockTab[i].con_num;
            }
            //判断剩余的空间是否足够分配
            if(freSpace<size){
                //剩余空间不够
                QMessageBox::critical(this,"分配失败",tr("剩余空间不足 ！"));
            }else{
                qDebug()<<"剩余空间足够，开始分配...";
                //寻找文件目录
                int id=-1;
                QString name;
                int indexBlockId;//索引块所在块号
                for (int i=0;i<indexFileDir.size();i++) {
                    if(indexFileDir[i].filename==filename){
                        id=indexFileDir[i].fileId;
                        name=indexFileDir[i].filename;
                        indexBlockId=indexFileDir[i].indexBlockId;
                        break;
                    }
                }
                //找到上一次分配的最后一个索引块____________________________
                int log=-1;
                while (indexBlockId!=-1) {
                    //索引块不结束
                    //寻找里面的每个磁盘块
                    qDebug()<<" 找到索引块 ："<<indexBlockId;
                    if(indexBlockId!=-1){
                        log=indexBlockId;
                    }
                    indexBlockId=indexFiles[indexBlockId].nextIndexBlockId;
                }
                qDebug()<<" 找到最后一个索引块 ："<<log;

                //__________________________________ 算法与第一次分配时一致
                int s=size/(blockNum*blockSize);//商
                int y=size%(blockNum*blockSize);//余
                qDebug()<<"商 : ______________________ "<<s;
                qDebug()<<"余 : ______________________ "<<y;

                int needIndexBlockNum;//所需要索引块数
                if(y>0){
                    needIndexBlockNum=s+1;
                }else{
                    needIndexBlockNum=s;
                }
                qDebug()<<"  开始索引分配  ："<<policy;
                qDebug()<<"------需要索引块数："<<needIndexBlockNum;
                int allnum=(ui->block_tab->rowCount())*(ui->block_tab->columnCount());
                //初始化虚拟磁盘
                for(int i=0;i<allnum;i++){
                    IndexBlock ind;
                    ind.diskblockId=i;
                    ind.fileId=-1;
                    ind.filename=nullptr;
                    ind.blocksID=new int[blockNum];
                    for(int j=0;j<blockNum;j++){
                        ind.blocksID[j]=-1;
                    }
                    ind.nextIndexBlockId=-1;
                    indexFiles.append(ind);
                }
                //修改文件大小
                int ms=size/blockSize;
                int my=size%blockSize;
                int needNum=my>0?(ms+1):ms;
                QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
                for (int i=0;i<pcb.size();i++) {
                    if(pcb[i]->name==filename){
                        pcb[i]->blockNum=pcb[i]->blockNum+needNum;
                    }
                }
                //新建文件目录
                IndexDir ifd;
                ifd.fileId=id;
                ifd.filename=name;
                ifd.size=size;
                //找到第一个空闲块存储起始索引块
                for(int i=0;i<diskBlock.size();i++){
                    if((diskBlock[i])->fileId==-1){
                        int rr=i/(ui->block_tab->columnCount());
                        int cc=i%(ui->block_tab->columnCount());
                        //qDebug()<<"找到起始索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc;
                        //设置文件目录的起始索引块位置 并添加到目录中
                        ifd.indexBlockId=i;
                        indexFileDir.append(ifd);
                        //将上一次最后一个索引块的末尾链接______________________________
                        indexFiles[log].nextIndexBlockId=i;

                        //为起始索引块分配磁盘
                        diskBlock[i]->fileId=id;
                        //修改颜色
                        QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        tt->setText(name+"(s)");
                        //修改空闲表___________________________________
                        this->disrtSpaceInFreeTable(i);
                        //____________________________________________
                        break;
                    }
                }
                int hasgetSpace=0;//已分配内存
                //创建起始索引块
                (indexFiles[ifd.indexBlockId]).fileId=id;
                (indexFiles[ifd.indexBlockId]).filename=name;
                if(needIndexBlockNum<=1){
                    qDebug()<<"------------- 小文件索引分配 -------------";
                    //小文件：只需要一个索引块
                    int ss=size/blockSize;
                    int yy=size%blockSize;
                    int needCNum;//需要磁盘块数
                    if(yy>0){
                        needCNum=ss+1;
                    }else{
                        needCNum=ss;
                    }
                    qDebug()<<"---需要磁盘块数："<<needCNum;
                    for(int j=0;j<needCNum;j++){
                        //对起始索引块中寻找每个磁盘块
                        qDebug()<<"给 起始索引块 中磁盘块 __  "<<j<<" 分配";
                        bool cipanHasget=false;//改索引块中改磁盘是否分配完成
                        for(int i=0;i<diskBlock.size();i++){
                            if(hasgetSpace>=size){
                                qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                cipanHasget=true;
                                break;
                            }
                            else if(diskBlock[i]->fileId==-1){
                                qDebug()<<"找到空闲块 ："<<i;
                                hasgetSpace=hasgetSpace+blockSize;
                                indexFiles[ifd.indexBlockId].blocksID[j]=i;
                                qDebug()<<"给索引块 "<<ifd.indexBlockId<<" 分配空闲块 ：***** "<<i<<" 已分配:"<<hasgetSpace;
                                //修改索引块 i 的属性
                                indexFiles[i].fileId=id;
                                indexFiles[i].filename=name;
                                //修改磁盘块文件 id 及颜色
                                diskBlock[i]->fileId=id;

                                int rrr=i/(ui->block_tab->columnCount());
                                int ccc=i%(ui->block_tab->columnCount());
                                QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                tt->setText(QString::number(id));

                                qDebug()<<"起始索引块 中磁盘块 __  "<<j<<"分配完成 *********** "<<hasgetSpace;
                                cipanHasget=true;
                                //修改空闲表
                                this->disrtSpaceInFreeTable(i);
                                break;
                            }
                        }
                    }
                    //起始索引块中 索引块指针为默认值: -1

                }else{//大文件，需要多个索引块
                    qDebug()<<"------------- 大文件索引分配 -------------";
                    //分配 起始索引块 中每个磁盘块
                    qDebug()<<"分配 起始索引块 中每个磁盘块 ____________";
                    for(int j=0;j<blockNum;j++){
                        qDebug()<<"给 起始索引块 中磁盘块 __  "<<j<<" 分配";
                       //对起始索引块中寻找每个磁盘块
                        for(int i=0;i<diskBlock.size();i++){
                            if(hasgetSpace>=size){
                                qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                break;
                            }
                            else if(diskBlock[i]->fileId==-1){
                                qDebug()<<"找到空闲块 ："<<i;
                                hasgetSpace=hasgetSpace+blockSize;
                                indexFiles[ifd.indexBlockId].blocksID[j]=i;
                                qDebug()<<"给起始索引块 "<<ifd.indexBlockId<<" 分配空闲块 ：***** "<<i;
                                //修改索引块 i 的属性
                                indexFiles[i].fileId=id;
                                indexFiles[i].filename=name;
                                //修改磁盘块文件 id 及颜色
                                diskBlock[i]->fileId=id;

                                int rrr=i/(ui->block_tab->columnCount());
                                int ccc=i%(ui->block_tab->columnCount());
                                QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                tt->setText(QString::number(id));
                                //修改空闲表
                                this->disrtSpaceInFreeTable(i);

                                qDebug()<<"起始索引块 "<<" 中磁盘块 __  "<<j<<" 分配完成 *********** "<<hasgetSpace;
                                break;
                            }
                        }

                    }
                    qDebug()<<"继续寻找下一个索引块 *********** ";
                    int lastPos=ifd.indexBlockId;//所在块号
                    int end=-2;//终止块号
                    // s   y
                    // 继续寻找下一个索引块
                    while (hasgetSpace<size) {
                        //索引块数量不够，继续分配
                        //寻找第一个空闲块存储作为下一个索引块
                        int nextIndBlockId;//下一个索引块所在 id
                        for(int i=0;i<diskBlock.size();i++){
                            if((diskBlock[i])->fileId==-1){
                                int rr=i/(ui->block_tab->columnCount());
                                int cc=i%(ui->block_tab->columnCount());
                                qDebug()<<"找到下一索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc;
                                nextIndBlockId=i;
                                //为索引块分配磁盘
                                diskBlock[i]->fileId=id;
                                //修改颜色
                                QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                tt->setText(name+"(s)");
                                //修改空闲表
                                this->disrtSpaceInFreeTable(i);

                                break;
                            }
                        }
                        //修改索引块首尾指针
                        qDebug()<<"修改了上一个索引块 ："<<lastPos<<" 为："<<nextIndBlockId;
                        (indexFiles[lastPos]).nextIndexBlockId=nextIndBlockId;
                        lastPos=nextIndBlockId;

                        //修改下一个索引块属性
                        (indexFiles[lastPos]).fileId=id;
                        (indexFiles[lastPos]).filename=name;

                        //对下一个索引块中寻找每个磁盘块
                        int sss=(size-hasgetSpace)/blockSize;
                        int yyy=(size-hasgetSpace)%blockSize;
                        int needCNumm;//需要磁盘块数
                        if(yyy>0){
                            needCNumm=sss+1;
                        }else{
                            needCNumm=sss;
                        }
                        qDebug()<<"还剩 "<<(size-hasgetSpace)<<"未分配";
                        qDebug()<<"这个索引块中需要磁盘块数 ： "<<needCNumm;
                        if(needCNumm>=blockNum){
                            //一个索引块中最多只能分配 blockNum 个磁盘块
                            //寻找 blockNum 个磁盘块
                            for(int k=0;k<blockNum;k++){
                                qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配";
                                for(int i=0;i<diskBlock.size();i++){
                                    if(hasgetSpace>=size){
                                        qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                        break;
                                    }
                                    if(diskBlock[i]->fileId==-1){
                                        qDebug()<<"找到空闲块 ："<<i;
                                        hasgetSpace=hasgetSpace+blockSize;
                                        indexFiles[nextIndBlockId].blocksID[k]=i;
                                        qDebug()<<"给索引块 "<<nextIndBlockId<<" 分配空闲块 ：***** "<<i;
                                        //修改索引块 i 的属性
                                        indexFiles[i].fileId=id;
                                        indexFiles[i].filename=name;
                                        //修改磁盘块文件 id 及颜色
                                        diskBlock[i]->fileId=id;

                                        int rrr=i/(ui->block_tab->columnCount());
                                        int ccc=i%(ui->block_tab->columnCount());
                                        qDebug()<<"修改盘块颜色  "<<rrr<<"  "<<ccc;
                                        QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                        tt->setText(QString::number(id));
                                        //修改空闲表
                                        this->disrtSpaceInFreeTable(i);

                                        qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配完成 ";
                                        break;
                                    }
                                }
                            }
                        }else{
                            // needCNumm<blockNum 只需要分配 needCNum 个磁盘
                            for(int k=0;k<needCNumm;k++){
                                qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配";
                                for(int i=0;i<diskBlock.size();i++){
                                    if(hasgetSpace>=size){
                                        qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                        break;
                                    }
                                    if(diskBlock[i]->fileId==-1){
                                        qDebug()<<"找到空闲块 ："<<i;
                                        hasgetSpace=hasgetSpace+blockSize;
                                        indexFiles[nextIndBlockId].blocksID[k]=i;
                                        qDebug()<<"给索引块 "<<nextIndBlockId<<" 分配空闲块 ：***** "<<i;
                                        //修改索引块 i 的属性
                                        indexFiles[i].fileId=id;
                                        indexFiles[i].filename=name;
                                        //修改磁盘块文件 id 及颜色
                                        diskBlock[i]->fileId=id;

                                        int rrr=i/(ui->block_tab->columnCount());
                                        int ccc=i%(ui->block_tab->columnCount());
                                        qDebug()<<"修改盘块颜色  "<<rrr<<"  "<<ccc;
                                        QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                        tt->setText(QString::number(id));
                                        //修改空闲表
                                        this->disrtSpaceInFreeTable(i);

                                        qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配完成 ";
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }



            }
        }
    }
}

void Disk::fileToFew(QString policy, QString filename, QString exbanSize)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k

    int fewSize=-exbanSize.toInt();
    qDebug()<<" "<<policy<<" 文件大小 "<<filename<<" 缩小 "<<fewSize<<" KB";

    int s=fewSize/blockSize;
    int y=fewSize%blockSize;
    //所需要释放的块数
    int releaseBlockNum=y>0?s+1:s;
    qDebug()<<"需要释放 "<<releaseBlockNum<<" 块=============";

    if(policy=="连续分配"){
        //找到文件总块数
        int allBlockNum;
        QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
        for (int i=0;i<pcb.size();i++) {
            if(pcb[i]->name==filename){
                allBlockNum=pcb[i]->blockNum;
                break;
            }
        }
        //目录内找到起始块
        int confBlockId;
        for (int i=0;i<confiledir.size();i++) {
            if(confiledir[i].filename==filename){
                confBlockId=confiledir[i].first_block;
            }
        }
        //文件块表内找到最后一个块记录 (下一扩展块值 -2)
        int bpt=confBlockId;//块遍历指针
        int hasgetNum=0;//已遍历块数
        int sxNum=allBlockNum;//剩下块数
        int endIndex=0;
        bool end=false;//是否遍历结束
        int logId;//
        int preBlockId=confBlockId;//上一个文件记录块块号
        int logInIndex;//块内号
        while (confBlockId!=-2) {
            if(end==true){
                break;
            }
            for (int i=0;i<fileBlocks.size();i++) {
                if(fileBlocks[i].firstBlockId==confBlockId){
                    //遍历每个文件块里面每个磁盘块
                    for (int j=fileBlocks[i].firstBlockId;j<=fileBlocks[i].firstBlockId+fileBlocks[i].blockNum-1;j++) {
                        logInIndex=j;
                        hasgetNum+=1;
                        bpt=j;
                        sxNum-=1;
                        qDebug()<<"遍历 i:"<<i<<"  j: "<<j<<"  sxNum ："<<sxNum<<" releaseBlockNum :"<<releaseBlockNum;
                        if(sxNum==releaseBlockNum){
                            preBlockId=confBlockId;
                            logId=i;
                            end=true;
                            break;
                        }
                    }
                    endIndex=i;
                    break;
                }
            }
            confBlockId=fileBlocks[endIndex].nextBlockId;
        }
        qDebug()<<"文件块记录里序号："<<logId<<" 上一个号 :"<<preBlockId<<" 块内索引："<<logInIndex;
        //文件块记录里找到该记录的上一个块索引
        int iindex;
        for (int i=0;i<fileBlocks.size();i++) {
            if(fileBlocks[i].nextBlockId==preBlockId){
                iindex=i;
                break;
            }
        }
        if(logInIndex==preBlockId){
            //刚好从这个记录块整体往后删除
            fileBlocks[iindex].nextBlockId=-2;
            //递归删除磁盘块
            int start=preBlockId;
            while (start!=-2) {
                for (int i=0;i<fileBlocks.size();i++) {
                    if(fileBlocks[i].firstBlockId==start){
                        //遍历每个文件块里面每个磁盘块
                        for (int j=fileBlocks[i].firstBlockId;j<=fileBlocks[i].firstBlockId+fileBlocks[i].blockNum-1;j++) {
                            //释放块
                            diskBlock[j]->fileId=-1;
                            diskBlock[j]->filename="";
                            this->releaseTabBlockColor(j);
                            this->flushFreeBlockTab();
                        }
                    }
                }
                start=fileBlocks[endIndex].nextBlockId;
            }
        }

    }else if(policy=="显式链接"){

    }else{

    }
}


void Disk::disrtSpaceInFreeTable(int nextId)
{
    //获取空闲空间列表表格控件
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    //修改空闲表对应记录_______________________
         //1.找到被分配块在空闲表中位置
    int freeIndex=-1;
    int Start=-1;
    int End=-1;
    for(int i=0;i<freeblockTab.size();i++){
        //该记录 i 中块起始号
        int s=freeblockTab[i].blockId;
        int e=freeblockTab[i].blockId+freeblockTab[i].con_num-1;
        if(nextId>=s&&nextId<=e){
            qDebug()<<"找到空闲表中对应记录 id:"<<s<<" e:"<<freeblockTab[i].con_num;
            freeIndex=i;
            Start=s;
            End=e;
            break;
        }
    }
        //2.考虑被分配块在空闲记录中的位置
    if(nextId==Start){
        //正好在开头
        qDebug()<<"待分配块正好在开头";
        freeblockTab[freeIndex].blockId=freeblockTab[freeIndex].blockId+1;
        freeblockTab[freeIndex].con_num=freeblockTab[freeIndex].con_num-1;
        if(freeblockTab[freeIndex].con_num==0){
            //分配完直接删除记录
            freeblockTab.erase(freeblockTab.begin()+freeIndex);
            ftb->removeRow(freeIndex);
        }else{
            //修改窗口表对应数据
            QTableWidgetItem *td0=ftb->item(freeIndex,0);
            td0->setText(QString::number(freeblockTab[freeIndex].blockId));
            QTableWidgetItem *td1=ftb->item(freeIndex,1);
            td1->setText(QString::number(freeblockTab[freeIndex].con_num));
        }

    }else if(nextId>Start&&nextId<End){
        //正好在中间,除了修改原有记录值，还要新增记录
        qDebug()<<"待分配块在中间";
        //修改原记录
        freeblockTab[freeIndex].con_num=(nextId-1)-freeblockTab[freeIndex].blockId+1;
        QTableWidgetItem *td1=ftb->item(freeIndex,1);
        td1->setText(QString::number(freeblockTab[freeIndex].con_num));
        //添加新记录
        FreeBlockTable nfrrt;
        nfrrt.blockId=nextId+1;
        nfrrt.con_num=End-nfrrt.blockId+1;
        freeblockTab.append(nfrrt);

        QTableWidgetItem *nd1=new QTableWidgetItem;
        nd1->setText(QString::number(nfrrt.blockId));
        ftb->setItem(freeIndex,0,nd1);
        QTableWidgetItem *nd2=new QTableWidgetItem;
        nd2->setText(QString::number(nfrrt.con_num));
        ftb->setItem(freeIndex,1,nd2);

    }else{
        //正好在结尾
        qDebug()<<"待分配块正好在结尾";
        freeblockTab[freeIndex].con_num=freeblockTab[freeIndex].con_num-1;
        //修改窗口表对应数据
        QTableWidgetItem *td1=ftb->item(freeIndex,1);
        td1->setText(QString::number(freeblockTab[freeIndex].con_num));
    }
    //___________________________

}

void Disk::setBlockBeFull(int start, int end)
{
    // 占用
    for (int i=start;i<=end;i++) {
        diskBlock[i]->fileId=-3;
        this->getBlockColor(i);
    }
}

void Disk::getSpaceToFolder()
{
    //给文件夹分配空间
    for (int i=0;i<diskBlock.size();i++) {
        if(diskBlock[i]->fileId==-1){
            qDebug()<<"//给文件夹分配空间 ::::::::::::::: "<<i;
            diskBlock[i]->fileId=-3;//文件夹
            //修改块颜色
            int rr=i/(this->getBlockTable()->rowCount());
            int cc=i%(this->getBlockTable()->columnCount());
            QTableWidgetItem *tt=this->getBlockTable()->item(rr,cc);
            tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
            tt->setText("-o");
            (this->freeblockTab).clear();
            this->flushFreeBlockTab();
            break;
        }
    }
}

QTableWidget *Disk::getBlockTable()
{
    return ui->block_tab;
}

void Disk::getBlockColor(int j)
{
    //清空磁盘块颜色
    int rr=(j)/(ui->block_tab->columnCount());
    int cc=(j)%(ui->block_tab->columnCount());
    QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
    tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
    tt->setText("");
}


Disk::~Disk()
{
    delete ui;
}

void Disk::distrcFileSpace(int id,QString name,int size,QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k

    qDebug()<<" 文件 "<<id<<"  "<<name<<" 分配 : "<<size<<" KB";
    qDebug()<<"------------------ 分配策略 ："<<policy;
    //获取空闲空间列表表格控件
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    //连续分配
    if(policy=="连续分配"){ // size (KB)
        this->conFileDistrc(id,name,size,policy);
    }else if(policy=="显式链接"){ // size (KB)
        this->fatFileDistrc(id,name,size,policy);
    }else{ // size (KB)
        qDebug()<<"空闲空间表 ————————————————————";
        this->indexFileDistrc(id,name,size,policy);
    }

}

//__________________________________________ 连续文件分配
void Disk::conFileDistrc(int id, QString name, int size, QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k
    //获取空闲空间列表表格控件
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    //连续分配
    if(freeblockTab.size()==0){
        //剩余空间不够
        QMessageBox::critical(this,"分配失败",tr("空闲表无可用空间记录 ！"));
    }else{
        //计算文件需要多少块
        int ms=size/blockSize;
        int my=size%blockSize;
        int needNum=my>0?(ms+1):ms;
        qDebug()<<" 需要 "<<needNum<<" 个磁盘块";
        bool hasConBlock=false;//是否有足够连续块
        //找到最小连续块
        int minConindex=-1;//最小连续块在空闲表中所在位置
        int conMinPos=-1;//连续块数最小起始块号
        int minConNum=1000000;//最小块数
        qDebug()<<"初始空闲空间表 ————————————————————";
        for(int i=0;i<freeblockTab.size();i++){
            if(freeblockTab[i].con_num>=needNum){
                //找到第一可足够连续块
                qDebug()<<" 找到  "<<i<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
                hasConBlock=true;
                minConindex=i;
                conMinPos=freeblockTab[i].blockId;
                minConNum=freeblockTab[i].con_num;
                break;
            }
        }
        qDebug()<<"找到最小连续块记录  index:"<<minConindex<<" blockId:"<<conMinPos<<" num:"<<minConNum;
        //文件块表添加记录
        FileBlock fb;
        fb.firstBlockId=conMinPos;
        fb.blockNum=needNum;
        fb.nextBlockId=-2;//  ***************** -2 表示文件结束
        fileBlocks.append(fb);

        if(hasConBlock==true){
            //修改文件大小
            QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
            for (int i=0;i<pcb.size();i++) {
                if(pcb[i]->name==name){
                    pcb[i]->blockNum=pcb[i]->blockNum+needNum;
                }
            }
            //添加文件目录
            conFileDir nconfdir;
            nconfdir.fileId=id;
            nconfdir.filename=name;
            nconfdir.size=size;
            nconfdir.first_block=conMinPos;
            confiledir.append(nconfdir);
            //文件策略表添加
            FilePolicy nf;
            nf.fileId=id;
            nf.filename=name;
            nf.policy=policy;
            fpol.append(nf);
            qDebug()<<"开始连续分配 ："<<policy;
            //开始分配
            for(int i=conMinPos;i<conMinPos+needNum;i++){
                qDebug()<<" 分配空闲块 ："<<i;
                //设置磁盘块的文件 id
                diskBlock[i]->fileId=id;
                //修改块颜色
                int rr=i/(ui->block_tab->columnCount());
                int cc=i%(ui->block_tab->columnCount());
                QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                tt->setText(QString::number(diskBlock[i]->fileId));

            }
            if(minConNum==needNum){
                qDebug()<<"空闲表删除记录 "<<minConindex<<freeblockTab[minConindex].blockId<<freeblockTab[minConindex].con_num;
                freeblockTab.erase(freeblockTab.begin()+minConindex);
                ftb->removeRow(minConindex);
            }else{
                //空闲表修改记录
                qDebug()<<"空闲表修改记录 "<<minConindex<<freeblockTab[minConindex].blockId<<freeblockTab[minConindex].con_num;
                freeblockTab[minConindex].blockId=conMinPos+needNum;
                freeblockTab[minConindex].con_num=minConNum-needNum;

                qDebug()<<"空闲表修改记录为 ："<<conMinPos+needNum<<" "<<minConNum-needNum;
                QTableWidgetItem *td0=ftb->item(minConindex,0);
                td0->setText(QString::number(conMinPos+needNum));
                QTableWidgetItem *td1=ftb->item(minConindex,1);
                td1->setText(QString::number(minConNum-needNum));
            }
            qDebug()<<"更新后空闲空间表 ————————————————————";
            for(int i=0;i<freeblockTab.size();i++){
                qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
            }
        }else{
            //最小连续块不够
            QMessageBox::critical(this,"分配失败",tr("空闲表最小连续块不够 ！"));
        }
    }

}

void Disk::fatFileDistrc(int id, QString name, int size, QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k
    //获取空闲空间列表表格控件
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    if(freeblockTab.size()==0){
        //剩余空间不够
        QMessageBox::critical(this,"分配失败",tr("空闲表无可用空间记录 ！"));
    }else{
        qDebug()<<"空闲空间表 ————————————————————";
        int freSpace=0;//剩余总空间
        for(int i=0;i<freeblockTab.size();i++){
            qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
            freSpace=freSpace+blockSize*freeblockTab[i].con_num;
        }
        //判断剩余的空间是否足够分配
        if(freSpace<size){
            //剩余空间不够
            QMessageBox::critical(this,"分配失败",tr("剩余空间不足 ！"));
        }
        else{
            //根据空闲表初始化 FAT
            QVector<Block*>::iterator iter;
            for (iter=diskBlock.begin();iter!=diskBlock.end();iter++){
                FAT fitem;
                fitem.blockId=(*iter)->blockId;
                //FAT 每个块 V 值待从空闲表定,初始值为 -2
                fitem.value=-2;
                Fat.append(fitem);
            }
            //空闲表有空闲空间记录，将对应 FAT 块 v 置 -1
            for(int i=0;i<freeblockTab.size();i++){
                int startBlockId=freeblockTab[i].blockId;
                int conNum=freeblockTab[i].con_num;
               // qDebug()<<"找到连续块记录： bid:"<<startBlockId<<" conN:"<<conNum;
                for (int j=startBlockId;j<=startBlockId+conNum-1;j++) {
                    (Fat[j]).value=-1;
                }
            }
            //查看 FAT
            for(int i=0;i<Fat.size();i++){
                qDebug()<<"FAT 记录 ："<<i<<" v: "<<Fat[i].value;
            }
            //修改文件大小
            int s=size/blockSize;
            int y=size%blockSize;
            int needNum=y>0?s+1:s;
            QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
            for (int i=0;i<pcb.size();i++) {
                if(pcb[i]->name==name){
                    pcb[i]->blockNum=pcb[i]->blockNum+needNum;
                }
            }

            //文件策略加入
            FilePolicy po;
            po.fileId=id;
            po.filename=name;
            po.policy=policy;
            fpol.append(po);
            qDebug()<<"开始显式链接分配 FAT ："<<policy;
            int firstId=-1;
            //初始化文件目录,首块号待定
            Dir_fat dir;
            dir.size=size;
            dir.fileId=id;
            dir.filename=name;

            int hasgetSpace=0;
            int nextId=-2;

            bool isFirst=true;//当前寻找是否第一空闲块
            while (hasgetSpace<size) {
                //寻找第一个空闲块
                for (int i=0;i<Fat.size();i++){
                    if(Fat[i].value==-1){
                        nextId=Fat[i].blockId;//找到第一空闲块 i
                        if(isFirst){
                            qDebug()<<"第一连续块 找到首块 ：_____ "<<i;
                            //当前寻找的为第一空闲块
                            dir.first_block=Fat[i].blockId;
                            isFirst=false;
                            //将目录信息加入
                            fatDir.append(dir);
                            //qDebug()<<"文件目录 "<<dir.fileId<<" "<<dir.filename<<" "<<dir.size<<" "<<dir.first_block;
                        }
                        break;
                    }
                }
                qDebug()<<"继续分配 块 : "<<nextId;
                //在空闲表中移除块 nextId;
                this->disrtSpaceInFreeTable(nextId);
                //修改磁盘块文件 id
                diskBlock[nextId]->fileId=id;
                //把下一个空闲块加入
                for (int i=0;i<Fat.size();i++){
                    if(Fat[i].blockId==firstId){
                        Fat[i].value=nextId;
                        break;
                    }
                }
                //文件尾部空闲块 FAT v 置 -2
                for (int i=0;i<Fat.size();i++){
                    if(Fat[i].blockId==nextId){
                        Fat[i].value=-2;
                        break;
                    }
                }
                int rcl=nextId/(ui->block_tab->columnCount());
                int ccl=nextId%(ui->block_tab->columnCount());
                QTableWidgetItem *tb=ui->block_tab->item(rcl,ccl);
                tb->setText(QString::number(id));
                tb->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                hasgetSpace=hasgetSpace+10;
                qDebug()<<"已分配 : "<<hasgetSpace;

                firstId=nextId;
                nextId=-2;
            }
            qDebug()<<"------------- 分配完成 !--------------";
            //查看 FAT
            for(int i=0;i<Fat.size();i++){
                qDebug()<<"FAT 记录 ："<<i<<" v: "<<Fat[i].value;
            }
        }

    }
}

void Disk::indexFileDistrc(int id, QString name, int size, QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k
    //获取空闲空间列表表格控件
    QTableWidget *ftb=freeblobktab->getFreeBlockTable();
    int freSpace=0;//剩余总空间
    for(int i=0;i<freeblockTab.size();i++){
        qDebug()<<" "<<freeblockTab[i].blockId<<"  "<<freeblockTab[i].con_num;
        freSpace=freSpace+blockSize*freeblockTab[i].con_num;
    }
    //判断剩余的空间是否足够分配
    if(freSpace<size){
        //剩余空间不够
        QMessageBox::critical(this,"分配失败",tr("剩余空间不足 ！"));
    }else{
        qDebug()<<"剩余空间足够，开始分配...";
        //
        //修改文件大小
        int st=size/blockSize;
        int yt=size%blockSize;
        int needNum=yt>0?st+1:st;
        QVector<myFilepro*> pcb=*(w->fileTab->getFileContext());
        for (int i=0;i<pcb.size();i++) {
            if(pcb[i]->name==name){
                pcb[i]->blockNum=pcb[i]->blockNum+needNum;
            }
        }

        //文件策略表添加
        FilePolicy nf;
        nf.fileId=id;
        nf.filename=name;
        nf.policy=policy;
        fpol.append(nf);

        int s=size/(blockNum*blockSize);//商
        int y=size%(blockNum*blockSize);//余
        qDebug()<<"商 : ______________________ "<<s;
        qDebug()<<"余 : ______________________ "<<y;

        int needIndexBlockNum;//所需要索引块数
        if(y>0){
            needIndexBlockNum=s+1;
        }else{
            needIndexBlockNum=s;
        }
        qDebug()<<"  开始索引分配  ："<<policy;
        qDebug()<<"------需要索引块数："<<needIndexBlockNum;
        int allnum=(ui->block_tab->rowCount())*(ui->block_tab->columnCount());
        //初始化虚拟磁盘
        for(int i=0;i<allnum;i++){
            IndexBlock ind;
            ind.diskblockId=i;
            ind.fileId=-1;
            ind.filename=nullptr;
            ind.blocksID=new int[blockNum];
            for(int j=0;j<blockNum;j++){
                ind.blocksID[j]=-1;
            }
            ind.nextIndexBlockId=-1;
            indexFiles.append(ind);
        }
        //新建文件目录
        IndexDir ifd;
        ifd.fileId=id;
        ifd.filename=name;
        ifd.size=size;
        //找到第一个空闲块存储起始索引块
        for(int i=0;i<diskBlock.size();i++){
            if((diskBlock[i])->fileId==-1){
                int rr=i/(ui->block_tab->columnCount());
                int cc=i%(ui->block_tab->columnCount());
                //qDebug()<<"找到起始索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc;
                //设置文件目录的起始索引块位置 并添加到目录中
                ifd.indexBlockId=i;
                indexFileDir.append(ifd);

                //为起始索引块分配磁盘
                diskBlock[i]->fileId=id;
                //修改颜色
                QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                tt->setText(name+"(s)");
                //修改空闲表___________________________________
                this->disrtSpaceInFreeTable(i);
                //____________________________________________
                break;
            }
        }
        int hasgetSpace=0;//已分配内存
        //创建起始索引块
        (indexFiles[ifd.indexBlockId]).fileId=id;
        (indexFiles[ifd.indexBlockId]).filename=name;
        if(needIndexBlockNum<=1){
            qDebug()<<"------------- 小文件索引分配 -------------";
            //小文件：只需要一个索引块
            int ss=size/blockSize;
            int yy=size%blockSize;
            int needCNum;//需要磁盘块数
            if(yy>0){
                needCNum=ss+1;
            }else{
                needCNum=ss;
            }
            qDebug()<<"---需要磁盘块数："<<needCNum;
            for(int j=0;j<needCNum;j++){
                //对起始索引块中寻找每个磁盘块
                qDebug()<<"给 起始索引块 中磁盘块 __  "<<j<<" 分配";
                bool cipanHasget=false;//改索引块中改磁盘是否分配完成
                for(int i=0;i<diskBlock.size();i++){
                    if(hasgetSpace>=size){
                        qDebug()<<"分配完成 *********** "<<hasgetSpace;
                        cipanHasget=true;
                        break;
                    }
                    else if(diskBlock[i]->fileId==-1){
                        qDebug()<<"找到空闲块 ："<<i;
                        hasgetSpace=hasgetSpace+blockSize;
                        indexFiles[ifd.indexBlockId].blocksID[j]=i;
                        qDebug()<<"给索引块 "<<ifd.indexBlockId<<" 分配空闲块 ：***** "<<i<<" 已分配:"<<hasgetSpace;
                        //修改索引块 i 的属性
                        indexFiles[i].fileId=id;
                        indexFiles[i].filename=name;
                        //修改磁盘块文件 id 及颜色
                        diskBlock[i]->fileId=id;

                        int rrr=i/(ui->block_tab->columnCount());
                        int ccc=i%(ui->block_tab->columnCount());
                        QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        tt->setText(QString::number(id));

                        qDebug()<<"起始索引块 中磁盘块 __  "<<j<<"分配完成 *********** "<<hasgetSpace;
                        cipanHasget=true;
                        //修改空闲表
                        this->disrtSpaceInFreeTable(i);
                        break;
                    }
                }
            }
            //起始索引块中 索引块指针为默认值: -1

        }else{//大文件，需要多个索引块
            qDebug()<<"------------- 大文件索引分配 -------------";
            //分配 起始索引块 中每个磁盘块
            qDebug()<<"分配 起始索引块 中每个磁盘块 ____________";
            for(int j=0;j<blockNum;j++){
                qDebug()<<"给 起始索引块 中磁盘块 __  "<<j<<" 分配";
               //对起始索引块中寻找每个磁盘块
                for(int i=0;i<diskBlock.size();i++){
                    if(hasgetSpace>=size){
                        qDebug()<<"分配完成 *********** "<<hasgetSpace;
                        break;
                    }
                    else if(diskBlock[i]->fileId==-1){
                        qDebug()<<"找到空闲块 ："<<i;
                        hasgetSpace=hasgetSpace+blockSize;
                        indexFiles[ifd.indexBlockId].blocksID[j]=i;
                        qDebug()<<"给起始索引块 "<<ifd.indexBlockId<<" 分配空闲块 ：***** "<<i;
                        //修改索引块 i 的属性
                        indexFiles[i].fileId=id;
                        indexFiles[i].filename=name;
                        //修改磁盘块文件 id 及颜色
                        diskBlock[i]->fileId=id;

                        int rrr=i/(ui->block_tab->columnCount());
                        int ccc=i%(ui->block_tab->columnCount());
                        QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        tt->setText(QString::number(id));
                        //修改空闲表
                        this->disrtSpaceInFreeTable(i);

                        qDebug()<<"起始索引块 "<<" 中磁盘块 __  "<<j<<" 分配完成 *********** "<<hasgetSpace;
                        break;
                    }
                }

            }
            qDebug()<<"继续寻找下一个索引块 *********** ";
            int lastPos=ifd.indexBlockId;//所在块号
            int end=-2;//终止块号
            // s   y
            // 继续寻找下一个索引块
            while (hasgetSpace<size) {
                //索引块数量不够，继续分配
                //寻找第一个空闲块存储作为下一个索引块
                int nextIndBlockId;//下一个索引块所在 id
                for(int i=0;i<diskBlock.size();i++){
                    if((diskBlock[i])->fileId==-1){
                        int rr=i/(ui->block_tab->columnCount());
                        int cc=i%(ui->block_tab->columnCount());
                        qDebug()<<"找到下一索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc;
                        nextIndBlockId=i;
                        //为索引块分配磁盘
                        diskBlock[i]->fileId=id;
                        //修改颜色
                        QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
                        tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                        tt->setText(name+"(s)");
                        //修改空闲表
                        this->disrtSpaceInFreeTable(i);

                        break;
                    }
                }
                //修改索引块首尾指针
                qDebug()<<"修改了上一个索引块 ："<<lastPos<<" 为："<<nextIndBlockId;
                (indexFiles[lastPos]).nextIndexBlockId=nextIndBlockId;
                lastPos=nextIndBlockId;

                //修改下一个索引块属性
                (indexFiles[lastPos]).fileId=id;
                (indexFiles[lastPos]).filename=name;

                //对下一个索引块中寻找每个磁盘块
                int sss=(size-hasgetSpace)/blockSize;
                int yyy=(size-hasgetSpace)%blockSize;
                int needCNumm;//需要磁盘块数
                if(yyy>0){
                    needCNumm=sss+1;
                }else{
                    needCNumm=sss;
                }
                qDebug()<<"还剩 "<<(size-hasgetSpace)<<"未分配";
                qDebug()<<"这个索引块中需要磁盘块数 ： "<<needCNumm;
                if(needCNumm>=blockNum){
                    //一个索引块中最多只能分配 blockNum 个磁盘块
                    //寻找 blockNum 个磁盘块
                    for(int k=0;k<blockNum;k++){
                        qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配";
                        for(int i=0;i<diskBlock.size();i++){
                            if(hasgetSpace>=size){
                                qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                break;
                            }
                            if(diskBlock[i]->fileId==-1){
                                qDebug()<<"找到空闲块 ："<<i;
                                hasgetSpace=hasgetSpace+blockSize;
                                indexFiles[nextIndBlockId].blocksID[k]=i;
                                qDebug()<<"给索引块 "<<nextIndBlockId<<" 分配空闲块 ：***** "<<i;
                                //修改索引块 i 的属性
                                indexFiles[i].fileId=id;
                                indexFiles[i].filename=name;
                                //修改磁盘块文件 id 及颜色
                                diskBlock[i]->fileId=id;

                                int rrr=i/(ui->block_tab->columnCount());
                                int ccc=i%(ui->block_tab->columnCount());
                                qDebug()<<"修改盘块颜色  "<<rrr<<"  "<<ccc;
                                QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                tt->setText(QString::number(id));
                                //修改空闲表
                                this->disrtSpaceInFreeTable(i);

                                qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配完成 ";
                                break;
                            }
                        }
                    }
                }else{
                    // needCNumm<blockNum 只需要分配 needCNum 个磁盘
                    for(int k=0;k<needCNumm;k++){
                        qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配";
                        for(int i=0;i<diskBlock.size();i++){
                            if(hasgetSpace>=size){
                                qDebug()<<"分配完成 *********** "<<hasgetSpace;
                                break;
                            }
                            if(diskBlock[i]->fileId==-1){
                                qDebug()<<"找到空闲块 ："<<i;
                                hasgetSpace=hasgetSpace+blockSize;
                                indexFiles[nextIndBlockId].blocksID[k]=i;
                                qDebug()<<"给索引块 "<<nextIndBlockId<<" 分配空闲块 ：***** "<<i;
                                //修改索引块 i 的属性
                                indexFiles[i].fileId=id;
                                indexFiles[i].filename=name;
                                //修改磁盘块文件 id 及颜色
                                diskBlock[i]->fileId=id;

                                int rrr=i/(ui->block_tab->columnCount());
                                int ccc=i%(ui->block_tab->columnCount());
                                qDebug()<<"修改盘块颜色  "<<rrr<<"  "<<ccc;
                                QTableWidgetItem *tt=ui->block_tab->item(rrr,ccc);
                                tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
                                tt->setText(QString::number(id));
                                //修改空闲表
                                this->disrtSpaceInFreeTable(i);

                                qDebug()<<"给 索引块 "<<nextIndBlockId<<" 中磁盘块 __  "<<k<<" 分配完成 ";
                                break;
                            }
                        }
                    }
                }
            }
        }

    }
}

void Disk::geentFileSeekWind(QString filename,QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k

    QVector<int>cyIDorder;
    if(policy=="连续分配"){
        for (int i=0;i<fileBlocks.size();i++) {
            //对每个文件块，找里面的每个磁盘块
            for (int j=fileBlocks[i].firstBlockId;j<=fileBlocks[i].firstBlockId+fileBlocks[i].blockNum-1;j++) {
                cyIDorder.append(diskBlock[j]->cyId);
            }
        }
        //将磁道序列传给调度窗口
        showFileSeekWind->fileDispatch(cyIDorder);

    }else if(policy=="显式链接"){
        //找到文件的块号
        int BlockId;
        for (int i=0;i<fatDir.size();i++) {
            if(fatDir[i].filename==filename){
                BlockId=fatDir[i].first_block;
            }
        }
        while (BlockId!=-2) {
            cyIDorder.append(diskBlock[BlockId]->cyId);
            qDebug()<<" 块号 :"<<BlockId<<" 磁道："<<diskBlock[BlockId]->cyId;
            BlockId=Fat[BlockId].value;
        }
        //将磁道序列传给调度窗口
        showFileSeekWind->fileDispatch(cyIDorder);
    }else{
        //索引文件目录中找到起始索引块
        int indexBlockId;//索引块所在块号
        qDebug()<<"索引文件目录表大小 "<<indexFileDir.size();
        for (int i=0;i<indexFileDir.size();i++) {
            if(indexFileDir[i].filename==filename){
                indexBlockId=indexFileDir[i].indexBlockId;
                break;
            }
        }
        while (indexBlockId!=-1) {
            //索引块不结束
            //寻找里面的每个磁盘块
            for (int i=0;i<blockNum;i++) {
                if(indexFiles[indexBlockId].blocksID[i]==-1){
                    break;
                }else{
                    qDebug()<<"磁盘块 ："<<indexFiles[indexBlockId].blocksID[i]<<" 磁道号："<<diskBlock[indexFiles[indexBlockId].blocksID[i]]->cyId;
                    cyIDorder.append(diskBlock[indexFiles[indexBlockId].blocksID[i]]->cyId);
                }
            }
            indexBlockId=indexFiles[indexBlockId].nextIndexBlockId;
        }
    }

    //____________________
    w->processTab->Create_Process_For_File(cyIDorder);
}

void Disk::releaseSpace(QString filename, QString policy)
{
    int blockSize=10;//块大小
    int blockNum=3;//每个索引块包含磁盘块数 30k
    qDebug()<<"释放 "<<policy<<" 文件 "<<filename;
    //_______________________________
    //文件策略表删除
    for (int i=0;i<fpol.size();i++) {
        if((fpol[i]).filename==filename){
            qDebug()<<" remove policy ____________________ "<<fpol[i].filename;
            fpol.remove(i);
            break;
        }
    }
    //删除文件目录
    if(policy=="连续分配"){
        int firstBlockId;//找到起始块
        //删除连续文件目录
        qDebug()<<" remove dir ................ ";
        for (int i=0;i<confiledir.size();i++) {
            if((confiledir[i]).filename==filename){
                qDebug()<<" 找到起始块  "<<confiledir[i].first_block;
                firstBlockId=confiledir[i].first_block;
                qDebug()<<" remove con_dir ____________________ "<<confiledir[i].filename;
                confiledir.remove(i);
                break;
            }
        }
        //根据起始块迭代找磁盘块
        while (firstBlockId!=-2) {
            for (int i=0;i<fileBlocks.size();i++) {
                //文件块表里找到 文件块
                if(fileBlocks[i].firstBlockId==firstBlockId){
                    //设置该连续块里的每个磁盘块
                    for (int j=fileBlocks[i].firstBlockId;j<fileBlocks[i].firstBlockId+fileBlocks[i].blockNum;j++) {
                        qDebug()<<"开始释放块 : ____________ "<<j;
                        //清空磁盘块
                        diskBlock[j]->fileId=-1;
                        //修改磁盘块颜色
                        this->releaseTabBlockColor(j);
                        //重置空闲空间列表数据_________
                        freeblockTab.clear();
                        this->flushFreeBlockTab();
                        //删除文件 PCB
                        QVector<myFilepro*> *pcbList=w->fileTab->getFileContext();
                        for (int i=0;i<pcbList->size();i++) {
                            if(((*pcbList)[i])->name==filename){
                                (*pcbList).remove(i);
                                break;
                            };

                        }
                    }
                    qDebug()<<" i ： "<<i;
                    qDebug()<<" fileBlocks[i].firstBlockId ： "<<fileBlocks[i].firstBlockId;
                    firstBlockId=fileBlocks[i].nextBlockId;
                    qDebug()<<" firstBlockId 变为 ： "<<firstBlockId;
                }
            }
        }

    }else if(policy=="显式链接"){
        for (int i=0;i<fatDir.size();i++) {
            qDebug()<<" "<<fatDir[i].filename<<" "<<fatDir[i].first_block;
        }
        //找到文件的块号
        int BlockId;
        for (int i=0;i<fatDir.size();i++) {
            if(fatDir[i].filename==filename){
                qDebug()<<" 找到起始块 :"<<BlockId;
                BlockId=fatDir[i].first_block;
            }
        }
        //删除链接文件目录
        for (int i=0;i<fatDir.size();i++) {
            if((fatDir[i]).filename==filename){
                qDebug()<<" remove fat_dir ____________________ "<<fatDir[i].filename;
                fatDir.remove(i);
                break;
            }
        }
        while (BlockId!=-2) {
            qDebug()<<" 释放块 :"<<BlockId;
            //清空磁盘块
            diskBlock[BlockId]->fileId=-1;
            diskBlock[BlockId]->filename="";
            //修改磁盘块颜色
            this->releaseTabBlockColor(BlockId);
            //重置空闲空间列表
            freeblockTab.clear();
            this->flushFreeBlockTab();

            BlockId=Fat[BlockId].value;
        }

    }else{
        //索引文件目录中找到起始索引块
        int indexBlockId;//索引块所在块号
        qDebug()<<"索引文件目录表大小 "<<indexFileDir.size();
        for (int i=0;i<indexFileDir.size();i++) {
            if(indexFileDir[i].filename==filename){
                indexBlockId=indexFileDir[i].indexBlockId;
                break;
            }
        }
        //删除索引文件目录
        for (int i=0;i<indexFileDir.size();i++) {
            if(indexFileDir[i].filename==filename){
                indexFileDir.remove(i);
                break;
            }
        }
        while (indexBlockId!=-1) {
            //清空索引块所占块
            //清空磁盘块
            diskBlock[indexBlockId]->fileId=-1;
            diskBlock[indexBlockId]->filename="";
            //修改磁盘块颜色
            this->releaseTabBlockColor(indexBlockId);
            //重置空闲空间列表
            freeblockTab.clear();
            this->flushFreeBlockTab();
            //索引块不结束
            //寻找里面的每个磁盘块
            for (int i=0;i<blockNum;i++) {
                if(indexFiles[indexBlockId].blocksID[i]==-1){
                    break;
                }else{
                    qDebug()<<"磁盘块 ："<<indexFiles[indexBlockId].blocksID[i]<<" 磁道号："<<diskBlock[indexFiles[indexBlockId].blocksID[i]]->cyId;
                    //清空磁盘块
                    diskBlock[indexFiles[indexBlockId].blocksID[i]]->fileId=-1;
                    diskBlock[indexFiles[indexBlockId].blocksID[i]]->filename="";
                    //清空虚拟映射块
                    indexFiles[indexFiles[indexBlockId].blocksID[i]].fileId=-1;
                    indexFiles[indexFiles[indexBlockId].blocksID[i]].filename="";
                    //修改磁盘块颜色
                    this->releaseTabBlockColor(indexFiles[indexBlockId].blocksID[i]);
                    //重置空闲空间列表
                    freeblockTab.clear();
                    this->flushFreeBlockTab();
                }
            }
            qDebug()<<" indexBBBBB ++++++++++++=:"<<indexBlockId;
            qDebug()<<" indexFiles[indexBlockId].nextIndexBlockId++++++++++ :"<<indexFiles[indexBlockId].nextIndexBlockId;
            indexBlockId=indexFiles[indexBlockId].nextIndexBlockId;
        }
    }

    this->showBlockInfo();
    this->showFreeData();
}

void Disk::releaseTabBlockColor(int j)
{
    //清空磁盘块颜色
    int rr=(j)/(ui->block_tab->columnCount());
    int cc=(j)%(ui->block_tab->columnCount());
    QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
    tt->setBackground(QBrush(Qt::gray));
    tt->setText("");
}

void Disk::flushFreeBlockTab()
{
    //初始化空闲空间列表 初始状态为一个条目，紧跟数量为总块数
    int conBlockNum=0;//连续空闲块数量
    for(int i=0;i<diskBlock.size();i++){
        int start=-1;//每次连续块开头
        int end=-1;//每次连续块结尾
        int conNum=0;//每次连续块数
        if((diskBlock[i])->fileId==-1){//找到第一个空闲块
//            qDebug()<<"找到第一个空闲块 ："<<i;
            start=i;
            int con=0;
            for(int j=i;j<diskBlock.size();j++){//接着查询连续块是否足够
                if(j==diskBlock.size()-1){
                    //连续块结束
                    end=j;
                    con=con+1;
//                    qDebug()<<"连续块 "<<start<<"  "<<end<<"  结束 end eeeeeeeeee";
                    break;
                }
                if((diskBlock[j]->fileId!=-1)||(j==diskBlock.size()-1)){
                    //连续块结束
                    end=j-1;
//                    qDebug()<<"连续块 "<<start<<"  "<<end<<"  结束 end eeeeeeeeee";
                    break;
                }else{
                    con=con+1;//连续块数 +1
                }
            }
            conNum=con;

            FreeBlockTable fr;
            fr.blockId=start;
            fr.con_num=conNum;
            freeblockTab.append(fr);

            i=end+1;
            if(end+1==diskBlock.size()-1){
                break;
            }
            conBlockNum++;
        }
    }
}

void Disk::showBlockInfo()
{
    for(int i=0;i<diskBlock.size();i++){
       // qDebug()<<" bid: "<<diskBlock[i]->blockId<<" fid:"<<diskBlock[i]->fileId<<" fn:"<<diskBlock[i]->filename;
    }
}

void Disk::showFreeData()
{
    //___________________
    qDebug()<<"空闲空间列表 ;;;;;;;;;;;;;;;;;;;;;; ";
    for (int i=0;i<freeblockTab.size();i++) {
        qDebug()<<freeblockTab[i].blockId<<" "<<freeblockTab[i].con_num;
    }
}

void Disk::LookFileSpace(int id, QString name,QString type,QString pos,int size,QString time)
{
    qDebug()<<"开始查看文件空间详情 ------------ ";
    QString policy;
    for (int i=0;i<fpol.size();i++) {
        if(fpol[i].fileId==id&&fpol[i].filename==name){
            policy=fpol[i].policy;
            break;
        }
    }
    if(policy=="连续分配"){
        qDebug()<<"查看 连续分配文件 "<<id<<" "<<name<<" 详情";
        qDebug()<<"文件块 ： ";
        for(int i=0;i<fileBlocks.size();i++){
            qDebug()<<fileBlocks[i].firstBlockId<<" "<<fileBlocks[i].blockNum<<" "<<fileBlocks[i].nextBlockId;
        }
        confDetailWind=new conFileSpaceDetail;
        //***************** 文件目录结构 表
        QTableWidget *dtb=confDetailWind->getconfDirTableWidget();
        dtb->setColumnCount(4);
        dtb->setRowCount(1);
        //设置表头
        QTableWidgetItem *h1=new QTableWidgetItem("文件id");
        QFont font=h1->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(8);//字体大小
        h1->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h1->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(0,h1);

        QTableWidgetItem *h2=new QTableWidgetItem("文件名");
        h2->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h2->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(1,h2);

        QTableWidgetItem *h3=new QTableWidgetItem("文件大小");
        h3->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h3->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(2,h3);

        QTableWidgetItem *h4=new QTableWidgetItem("起始块");
        h4->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h4->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(3,h4);

        //加入数据
        //查找文件目录
        int first_block;//首块号
        for(int i=0;i<confiledir.size();i++){
            if(confiledir[i].fileId==id&&confiledir[i].filename==name){
                first_block=confiledir[i].first_block;
            }
        }
        QTableWidgetItem *bd1=new QTableWidgetItem;
        bd1->setText(QString::number(id));
        dtb->setItem(0,0,bd1);
        QTableWidgetItem *bd2=new QTableWidgetItem;
        bd2->setText(name);
        dtb->setItem(0,1,bd2);
        QTableWidgetItem *bd3=new QTableWidgetItem;
        bd3->setText(QString::number(size));
        dtb->setItem(0,2,bd3);
        QTableWidgetItem *bd4=new QTableWidgetItem;
        bd4->setText(QString::number(first_block));
        dtb->setItem(0,3,bd4);


        //***************** 文件块表
        QTableWidget *tbd=confDetailWind->getconfTableWidget();
        tbd->setColumnCount(3);
        tbd->setRowCount(fileBlocks.size());
        //设置表头
        QTableWidgetItem *h11=new QTableWidgetItem("连续块起始");
        QFont fontt=h11->font();//获取原有字体设置
        fontt.setBold(true);//设置为粗体
        fontt.setPointSize(8);//字体大小
        h11->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h11->setFont(fontt);//设置字体
        tbd->setHorizontalHeaderItem(0,h11);

        QTableWidgetItem *h22=new QTableWidgetItem("块数");
        h22->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h22->setFont(fontt);//设置字体
        tbd->setHorizontalHeaderItem(1,h22);

        QTableWidgetItem *h33=new QTableWidgetItem("扩展起始");
        h33->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h33->setFont(fontt);//设置字体
        tbd->setHorizontalHeaderItem(2,h33);
        //加入数据
        for (int i=0;i<fileBlocks.size();i++) {
            QTableWidgetItem *df1=new QTableWidgetItem;
            df1->setText(QString::number(fileBlocks[i].firstBlockId));
            tbd->setItem(i,0,df1);
            QTableWidgetItem *df2=new QTableWidgetItem;
            df2->setText(QString::number(fileBlocks[i].blockNum));
            tbd->setItem(i,1,df2);
            QTableWidgetItem *df3=new QTableWidgetItem;
            df3->setText(QString::number(fileBlocks[i].nextBlockId));
            tbd->setItem(i,2,df3);
        }
        confDetailWind->show();
    }
    else if(policy=="显式链接"){
        //查找文件目录
        int fileId;
        QString filename;
        int size;
        int first_block=-1;//首块号
        for (int i=0;i<fatDir.size();i++) {
            if(fatDir[i].fileId==id&&fatDir[i].filename==name){
                fileId=fatDir[i].fileId;
                QString filename=fatDir[i].filename;
                size=fatDir[i].size;
                first_block=fatDir[i].first_block;//首块号
                break;
            }
        }
        qDebug()<<" 找到文件  "<<name<<" 起始块 ："<<first_block;
        //** ****************************** 创建 FAT 表
        fatDetailWind=new fatFileSpaceDetail;

        //************** 创建文件目录表
        QTableWidget *dr=fatDetailWind->getFatDirTableWidget();
        dr->setColumnCount(4);
        dr->setRowCount(1);
        //设置表头
        QTableWidgetItem *dh1=new QTableWidgetItem("文件id");
        dh1->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;
        dr->setHorizontalHeaderItem(0,dh1);
        QTableWidgetItem *dh2=new QTableWidgetItem("文件名");
        dh2->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;
        dr->setHorizontalHeaderItem(1,dh2);
        QTableWidgetItem *dh3=new QTableWidgetItem("文件大小");
        dh3->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;
        dr->setHorizontalHeaderItem(2,dh3);
        QTableWidgetItem *dh4=new QTableWidgetItem("起始块");
        dh4->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;
        dr->setHorizontalHeaderItem(3,dh4);
        //添加数据
        QTableWidgetItem *d0=new QTableWidgetItem(QString::number(fileId));
        dr->setItem(0,0,d0);
        QTableWidgetItem *d1=new QTableWidgetItem(name);
        dr->setItem(0,1,d1);
        QTableWidgetItem *d2=new QTableWidgetItem(QString::number(size));
        dr->setItem(0,2,d2);
        QTableWidgetItem *d3=new QTableWidgetItem(QString::number(first_block));
        dr->setItem(0,3,d3);

        //*************
        QTableWidget *tb=fatDetailWind->getFatTableWidget();
        tb->setColumnCount(2);
        tb->setRowCount(Fat.size());
        //设置表头
        QTableWidgetItem *h1=new QTableWidgetItem("块号");
        QFont font=h1->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(12);//字体大小
        h1->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h1->setFont(font);//设置字体
        tb->setHorizontalHeaderItem(0,h1);

        QTableWidgetItem *h2=new QTableWidgetItem("FAT 值");
        QFont font1=h2->font();//获取原有字体设置
        font1.setBold(true);//设置为粗体
        font1.setPointSize(12);//字体大小
        h2->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h2->setFont(font1);//设置字体
        tb->setHorizontalHeaderItem(1,h2);

        for(int i=0;i<tb->rowCount();i++){
             tb->setColumnWidth(i,150);
        }
        for(int i=0;i<tb->columnCount();i++){
             tb->setRowHeight(i,50);
        }
        for (int i=0;i<Fat.size();i++) {
            QTableWidgetItem *bd=new QTableWidgetItem;
            bd->setText(QString::number(Fat[i].blockId));
            tb->setItem(i,0,bd);

            QTableWidgetItem *bv=new QTableWidgetItem;
            bv->setText(QString::number(Fat[i].value));
            tb->setItem(i,1,bv);
        }
        //修改 FAT 文件块颜色

        fatDetailWind->show();

    }
    else if(policy=="索引分配"){
        qDebug()<<"查看 索引分配文件 "<<id<<" "<<name<<" 详情";
        indexfDetailWind=new indexFileSpaceDetail;

        //***************** 文件目录结构 表
        QTableWidget *dtb=indexfDetailWind->getindexFileDirTable();
        dtb->setColumnCount(4);
        dtb->setRowCount(1);
        //设置表头
        QTableWidgetItem *h1=new QTableWidgetItem("文件id");
        QFont font=h1->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(8);//字体大小
        h1->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h1->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(0,h1);

        QTableWidgetItem *h2=new QTableWidgetItem("文件名");
        h2->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h2->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(1,h2);

        QTableWidgetItem *h3=new QTableWidgetItem("文件大小");
        h3->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h3->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(2,h3);

        QTableWidgetItem *h4=new QTableWidgetItem("起始索引块");
        h4->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h4->setFont(font);//设置字体
        dtb->setHorizontalHeaderItem(3,h4);

        //加入数据
        int fid;//起始索引块
        int nextb=-1;
        for (int i=0;i<indexFiles.size();i++) {
            if(indexFiles[i].fileId==id&&indexFiles[i].filename==name){
                fid=i;
                nextb=indexFiles[i].nextIndexBlockId;
                break;
            }
        }
        qDebug()<<"起始块 ：________ "<<fid;
        QTableWidgetItem *bd1=new QTableWidgetItem;
        bd1->setText(QString::number(id));
        dtb->setItem(0,0,bd1);
        QTableWidgetItem *bd2=new QTableWidgetItem;
        bd2->setText(name);
        dtb->setItem(0,1,bd2);
        QTableWidgetItem *bd3=new QTableWidgetItem;
        bd3->setText(QString::number(size));
        dtb->setItem(0,2,bd3);
        QTableWidgetItem *bd4=new QTableWidgetItem;
        bd4->setText(QString::number(fid));
        dtb->setItem(0,3,bd4);

        //***************** 文件块表
        QTableWidget *tgg=indexfDetailWind->getindexFileTable();
        tgg->setColumnCount(3);
        tgg->setRowCount(20);
        //设置表头
        QTableWidgetItem *h11=new QTableWidgetItem("索引块号");
        QFont fontt=h11->font();//获取原有字体设置
        fontt.setBold(true);//设置为粗体
        fontt.setPointSize(8);//字体大小
        h11->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h11->setFont(fontt);//设置字体
        tgg->setHorizontalHeaderItem(0,h11);

        QTableWidgetItem *h22=new QTableWidgetItem("磁盘块号");
        h22->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h22->setFont(fontt);//设置字体
        tgg->setHorizontalHeaderItem(1,h22);

        QTableWidgetItem *h33=new QTableWidgetItem("下一索引块");
        h33->setBackground(QColor(color1[6][0],color1[6][1],color1[6][2]));;//字体颜色
        h33->setFont(fontt);//设置字体
        tgg->setHorizontalHeaderItem(2,h33);
        //加入数据
        int indexNum=0;
        while (true) { //索引块不结束
            if(nextb==-1){
                break;
            }

            qDebug()<<"fid:"<<fid<<"  nextb:"<<nextb;
            qDebug()<<"表 r:"<<indexNum<<" c:"<<0<<" 添加 "<<fid;
            QTableWidgetItem *df1=new QTableWidgetItem;
            df1->setText(QString::number(fid));
            tgg->setItem(indexNum,0,df1);
            //三个磁盘块号
            QString bdd=QString::number(indexFiles[fid].blocksID[0])+","+QString::number(indexFiles[fid].blocksID[1])+","+QString::number(indexFiles[fid].blocksID[2]);
            qDebug()<<"表 r:"<<indexNum<<" c:"<<1<<" 添加 "<<bdd;
            QTableWidgetItem *c1=new QTableWidgetItem;
            c1->setText(bdd);
            tgg->setItem(indexNum,1,c1);

            qDebug()<<"表 r:"<<indexNum<<" c:"<<2<<" 添加 "<<indexFiles[fid].nextIndexBlockId;
            QTableWidgetItem *c2=new QTableWidgetItem;
            c2->setText(QString::number(indexFiles[fid].nextIndexBlockId));
            tgg->setItem(indexNum,2,c2);

            indexNum++;
            fid=nextb;
            qDebug()<<"fid:"<<fid;
            if(fid<0){
                break;
            }
            nextb=indexFiles[fid].nextIndexBlockId;
        }

        indexfDetailWind->show();
    }

}




/*****************************************************  磁盘调度  **************************************************************/
void Disk::ShowDiskTrack(int track_num){
    //初始磁盘块表格
    track_num--;
    for(int j=0;j<15;j++){
        QTableWidgetItem *t=ui->block_tab->item(track_num,j);
        //t->setText(QString::number(-1));
        //qDebug()<<"track num!!!!! ："<<track_num;
        t->setBackground(QBrush(Qt::red));
    }
 QCoreApplication::processEvents();
    QElapsedTimer t1;
    //延时1s
    t1.start();
    while(t1.elapsed()<500);
    int i;

    for(i=0;i < diskBlock.size();i++){
        int rr=i/(ui->block_tab->columnCount());
        int cc=i%(ui->block_tab->columnCount());
        QTableWidgetItem *tt=ui->block_tab->item(rr,cc);
        tt->setBackground(QBrush(Qt::gray));
        if((diskBlock[i])->fileId!=-1){
            //修改颜色
            tt->setBackground(QBrush(QColor(color1[6][0],color1[6][1],color1[6][2])));
            tt->setText(QString::number(diskBlock[i]->fileId));
//            qDebug()<<"找到起始索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc; qDebug()<<"找到起始索引块位置 ："<<i<<"  r: "<<rr<<" c:"<<cc;
        }
        if(i == 119) {
             break;
        };
    }
     QCoreApplication::processEvents();
}
