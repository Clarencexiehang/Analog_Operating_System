#include "confilespacedetail.h"
#include "ui_confilespacedetail.h"

conFileSpaceDetail::conFileSpaceDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::conFileSpaceDetail)
{
    ui->setupUi(this);
    this->setWindowTitle("连续文件空间详情");
    ui->dir_tab->verticalHeader()->setVisible(false);
    ui->fileblock_tab->verticalHeader()->setVisible(false);
}

conFileSpaceDetail::~conFileSpaceDetail()
{
    delete ui;
}

QTableWidget *conFileSpaceDetail::getconfTableWidget()
{
    return ui->fileblock_tab;
}

QTableWidget *conFileSpaceDetail::getconfDirTableWidget()
{
    return ui->dir_tab;
}
