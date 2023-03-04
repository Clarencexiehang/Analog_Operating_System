#include "confilespacedetail.h"
#include "ui_confilespacedetail.h"

conFileSpaceDetail::conFileSpaceDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::conFileSpaceDetail)
{
    ui->setupUi(this);
    this->setWindowTitle("连续文件空间详情");

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
