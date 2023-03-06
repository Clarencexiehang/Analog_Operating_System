#include "freeblocktable.h"
#include "ui_freeblocktable.h"

freeBlockTable::freeBlockTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::freeBlockTable)
{
    ui->setupUi(this);
    //***************** 空闲空间列表
    QTableWidget *tbd=ui->freeblock_tab;
    tbd->verticalHeader()->setVisible(false);
    tbd->setColumnCount(2);
    tbd->setRowCount(200);
    //设置表头
    QTableWidgetItem *h11=new QTableWidgetItem("连续块起始");
    QFont fontt=h11->font();//获取原有字体设置
    fontt.setBold(true);//设置为粗体
    fontt.setPointSize(8);//字体大小
    h11->setBackground(Qt::blue);//字体颜色
//    h11->setBackground(Qt::blue);
    h11->setFont(fontt);//设置字体
    tbd->setHorizontalHeaderItem(0,h11);
    QTableWidgetItem *h22=new QTableWidgetItem("块数");
    h22->setBackground(Qt::blue);;//字体颜色
    h22->setFont(fontt);//设置字体
    tbd->setHorizontalHeaderItem(1,h22);
}

freeBlockTable::~freeBlockTable()
{
    delete ui;
}

QTableWidget *freeBlockTable::getFreeBlockTable()
{
    return ui->freeblock_tab;
}
