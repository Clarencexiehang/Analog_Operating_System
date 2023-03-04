#include "indexfilespacedetail.h"
#include "ui_indexfilespacedetail.h"

indexFileSpaceDetail::indexFileSpaceDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::indexFileSpaceDetail)
{
    ui->setupUi(this);
    this->setWindowTitle("索引分配详情");
}

indexFileSpaceDetail::~indexFileSpaceDetail()
{
    delete ui;
}

QTableWidget *indexFileSpaceDetail::getindexFileTable()
{
    return ui->indexblock_tab;
}

QTableWidget *indexFileSpaceDetail::getindexFileDirTable()
{
    return ui->indexf_dir_tab;
}
