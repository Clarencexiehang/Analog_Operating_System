#ifndef DISK_H
#define DISK_H

#include <QWidget>
#include <QVector>
#include <QWidget>
#include<QDebug>
#include<QTabWidget>
#include<QVector>
#include<QTableWidgetItem>
#include<QBrush>
#include<QMessageBox>
#include"confilespacedetail.h"
#include"fatfilespacedetail.h"
#include"indexfilespacedetail.h"
#include"showfileseekoder.h"
#include"freeblocktable.h"
#include"process/processtab.h"
#include"file/myfilepro.h"

#include<queue>
#include<cmath>
#include "QTabWidget"
#include "vector"
#include"cstring"
#include "algorithm"

using namespace std;

namespace Ui {
class Disk;
}

struct virtualMemory{
    QString pid;
    int startIndex;
    int ranking;
    virtualMemory * nextBlock;
};


//文件策略表
struct FilePolicy{
    int fileId;
    QString filename;
    QString policy;
};
// **************** 连续分配 *********
//文件目录
struct conFileDir{
    int fileId;
    QString filename;
    int size;
    int first_block;//首块号
};
//文件块结构
struct FileBlock{
    int firstBlockId;//连续块起始地址
    int blockNum;//块数
    int nextBlockId;//下一连续块起始地址
};
// **************** 连续分配 *********


// **************** FAT *********
//FAT 文件目录
struct Dir_fat{
    int fileId;
    QString filename;
    int size;
    int first_block;//首块号
};
//FAT表
struct FAT{
  int blockId;
  int value;
};
// **************** FAT *********

// **************** 索引分配 *********
//文件目录结构
struct IndexDir{
    int fileId;//文件 id
    QString filename;//文件名
    int size;//文件大小
    int indexBlockId;//起始索引块位置
};
//索引块 ：包含文件信息、3 个磁盘块地址、下一索引块地址
struct IndexBlock{
    int diskblockId;//索引块所在磁盘块号
    int fileId;//文件 id
    QString filename;//文件名
    int *blocksID;
    int nextIndexBlockId;//下一索引块地址  -1 结束
};
// **************** 索引分配 *********

//磁盘块
struct Block{
    int blockId;//块号
    int cyId=0;//块所在磁道号
    int fileId;//被分配文件 id
    QString filename;//目标文件名
    int size;//目标文件大小
};

//空闲空间管理 ：计数法
struct FreeBlockTable{
    int blockId;//磁盘地址
    int con_num;//紧跟空闲数量
};



class Disk : public QWidget
{
    Q_OBJECT

public:
    explicit Disk(QWidget *parent = nullptr);
    virtualMemory * virtualMemoryBlock = nullptr;
    bool isUsed[20];
    void createVirtualMemoryBlock(QString pid,int ranking);
    void initvirtualMemory();
    void replacePage(QString pid,int page);
    void freeMemory(QString pid);

    int* disk_sheduling(QVector<int> disk_visit_Queue);
    void ShowDiskTrack(int track_num);
    ~Disk();



    //文件扩充
    void fileExband(QString policy,QString filename,QString exbandType,QString exbanSize);
    //文件扩大
    void fileToMore(QString policy,QString filename,QString exbanSize);
    //文件缩小
    void fileToFew(QString policy,QString filename,QString exbanSize);

    //磁盘分配文件内存
    void  distrcFileSpace(int id,QString name,int size,QString);

    //************************* 新加
    //连续分配
    void conFileDistrc(int id,QString name,int size,QString);
    //显示链接
    void fatFileDistrc(int id,QString name,int size,QString);
    //索引分配
    void indexFileDistrc(int id,QString name,int size,QString);
    //************************* 新加

    //获取文件磁盘块序列

    //生成磁盘调度窗口
    void geentFileSeekWind(QString filename,QString policy);
    //释放磁盘空间
    void releaseSpace(QString filename,QString policy);
    //在空闲表中分配磁盘块 nextId
    void disrtSpaceInFreeTable(int nextId);

    //************************* 新加
    //______________________________
    //设置占用块
    void setBlockBeFull(int start,int end);
    //给文件夹分配空间
    void getSpaceToFolder();
    //返回磁盘块表
    QTableWidget *getBlockTable();
    //占用磁盘块颜色
    void getBlockColor(int j);
    //释放磁盘块颜色
    void releaseTabBlockColor(int j);
    //初始化空闲空间表数据
    void flushFreeBlockTab();
    //显示磁盘块信息
    void showBlockInfo();
    //显示空闲空间数据
    void showFreeData();
    //************************* 新加




    QString diskID;

    QVector<Block*>diskBlock;//磁盘块列表

    //文件策略表
    QVector<FilePolicy> fpol;
    // **************** 连续分配 *********
    //文件目录
    QVector<conFileDir>confiledir;
    QVector<FileBlock>fileBlocks;
    // **************** 连续分配 *********

    QVector<FreeBlockTable>freeblockTab;//空闲空间列表

    // **************** FAT *********
    //FAT 目录列表
    QVector<Dir_fat>fatDir;
    QVector<FAT>Fat;
    // **************** FAT *********

    // **************** 索引分配 *********
    QVector<IndexDir>indexFileDir;//索引文件目录
    QVector<IndexBlock>indexFiles;//虚拟磁盘映射
    // **************** 索引分配 *********

    //窗口
    conFileSpaceDetail *confDetailWind;
    fatFileSpaceDetail *fatDetailWind;
    indexFileSpaceDetail *indexfDetailWind;
    showFileSeekOder *showFileSeekWind;//磁盘调度窗口
    freeBlockTable *freeblobktab;//空闲空间列表窗口

public slots:

   //查看某一文件的磁盘分配情况
   void LookFileSpace(int id,QString name,QString type,QString pos,int size,QString time);


private:
    Ui::Disk *ui;
};

#endif // DISK_H
