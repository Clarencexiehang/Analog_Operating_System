#ifndef DISK_H
#define DISK_H

#include <QWidget>
#include <QVector>

#include<queue>
#include<cmath>
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

private:
    Ui::Disk *ui;
};

#endif // DISK_H
