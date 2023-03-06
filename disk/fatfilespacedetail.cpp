#include "fatfilespacedetail.h"
#include "ui_fatfilespacedetail.h"

fatFileSpaceDetail::fatFileSpaceDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fatFileSpaceDetail)
{
    ui->setupUi(this);
    this->setWindowTitle("显式链接分配详情");
    ui->fat_tab->verticalHeader()->setVisible(false);
    ui->fat_dir_tab->verticalHeader()->setVisible(false);
}

fatFileSpaceDetail::~fatFileSpaceDetail()
{
    delete ui;
}

QTableWidget *fatFileSpaceDetail::getFatTableWidget()
{
    return ui->fat_tab;
}

QTableWidget *fatFileSpaceDetail::getFatDirTableWidget()
{
    return ui->fat_dir_tab;
}
