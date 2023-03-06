#ifndef MEMORY_H
#define MEMORY_H

#include <QPushButton>
#include<QMenuBar>
#include <QQueue>
namespace Ui {
class Memory;
}
struct freeMemeryBlock{
    int startIndex;
    int endIndex;
    int memeryBlockSize;
    freeMemeryBlock *nextBlock;
};
struct usedMemeryBlock{
//    int startIndex;
//    int endIndex;
    int memeryBlockSize;
    QString pid;
    int requestPageList[50];
    int blockSize = 0;
    int requestPageCount = 0;
    int ranking;
    int pageList[5][2];
    QQueue<int> FIFOPageQueue;
    usedMemeryBlock *nextBlock;
};
class Memory : public QWidget
{
    Q_OBJECT

public:
    explicit Memory(QWidget *parent = nullptr);
    usedMemeryBlock * usedMemeryList;
    freeMemeryBlock * freeMemeryList;
    int isUsed[400];
    int proCount = 0;
    void paintEvent(QPaintEvent *);
    void initMemery();
    void BubbleSort(freeMemeryBlock * &L);
    void mergeFreeMemery();
    void dye(struct usedMemeryBlock * block,int flag);
    bool requestMemery(int pageFrame,QString pid);
    void replacePageByLRU(QString pid,int page);
    void freeMemery(QString pid);
    void replacePage(QString pid,int page);
    void replacePageByFIFO(QString pid,int page);
    ~Memory();

private:
    Ui::Memory *ui;
};

#endif // MEMORY_H
