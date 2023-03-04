#include "showfileseekoder.h"
#include "ui_showfileseekoder.h"

showFileSeekOder::showFileSeekOder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showFileSeekOder)
{
    ui->setupUi(this);
}

showFileSeekOder::~showFileSeekOder()
{
    delete ui;
}

void showFileSeekOder::fileDispatch(QVector<int> blockCyIdOrder)
{
    // blockCyIdOrder 相当于数组,里面的元素为 int 行,依次为 磁道 号序列
    // 访问方式 blockCyIdOrder[下标]
    // 例如：输出每个元素
    qDebug()<<"连续文件 磁道号 ";
    for (int i=0;i<blockCyIdOrder.size();i++) {
        qDebug()<<blockCyIdOrder[i];
    }

}
