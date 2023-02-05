#include "file.h"
#include "ui_file.h"

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
}

File::~File()
{
    delete ui;
}
