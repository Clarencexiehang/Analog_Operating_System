#include "myprocessicon.h"
#include "ui_myprocessicon.h"

MyProcessIcon::MyProcessIcon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyProcessIcon)
{
    ui->setupUi(this);
}

MyProcessIcon::~MyProcessIcon()
{
    delete ui;
}
