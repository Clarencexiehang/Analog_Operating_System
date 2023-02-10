#ifndef MEMORY_H
#define MEMORY_H

#include <QPushButton>
#include<QMenuBar>
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
    int startIndex;
    int endIndex;
    int memeryBlockSize;
    QString pid;
    int requestPageList[50];
    int requestPageCount = 0;
    int pageList[5];

    usedMemeryBlock *nextBlock;
};
class Memory : public QWidget
{
    Q_OBJECT

public:
    explicit Memory(QWidget *parent = nullptr);
    usedMemeryBlock * usedMemeryList;
    freeMemeryBlock * freeMemeryList;
    void paintEvent(QPaintEvent *);
    void initMemery();
    void BubbleSort(freeMemeryBlock * &L);
    void mergeFreeMemery();
    void dye(struct usedMemeryBlock * block,int flag);
    bool requestMemery(int pageFrame,QString pid);
    void replacePageByLRU(QString pid,int page);
    void freeMemery(QString pid);
    ~Memory();

private:
    Ui::Memory *ui;
};

#endif // MEMORY_H
