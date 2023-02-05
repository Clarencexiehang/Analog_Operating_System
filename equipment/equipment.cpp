#include "equipment.h"
#include "ui_equipment.h"

Equipment::Equipment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Equipment)
{
    ui->setupUi(this);
}

Equipment::~Equipment()
{
    delete ui;
}
