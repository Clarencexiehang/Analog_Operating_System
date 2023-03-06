#include "myprocess.h"
#include "ui_myprocess.h"

myProcess::myProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myProcess)
{
    ui->setupUi(this);
    ui->blueEdit->setReadOnly(true);
    ui->pIdEdit->setReadOnly(true);
    ui->printEdit->setReadOnly(true);
    ui->mouseEdit->setReadOnly(true);
    ui->keyEdit->setReadOnly(true);
}

myProcess::~myProcess()
{
    delete ui;
}

void myProcess::on_backbutton_clicked()
{
    myProcess::close();
}
