#include "disk.h"
#include "ui_disk.h"

Disk::Disk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disk)
{
    ui->setupUi(this);
}

Disk::~Disk()
{
    delete ui;
}
