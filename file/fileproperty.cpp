#include "fileproperty.h"
#include "ui_fileproperty.h"

fileProperty::fileProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileProperty)
{
    ui->setupUi(this);
}

fileProperty::~fileProperty()
{
    delete ui;
}

void fileProperty::on_btn_close_clicked()
{
    this->close();
}

void fileProperty::on_size_detail_clicked()
{
    qDebug()<<"查看文件  ： ";
    emit this->diskLookFileSpace(this->id,this->name,this->type,this->pos,(this->size).toInt(),this->time);
    this->close();
}

void fileProperty::getFilepropertyInfo(int i, QString ty, QString n, QString p, QString s, QString t)
{
    this->id=i;
    this->type=ty;
    this->name=n;
    this->pos=p;
    this->size=s;
    this->time=t;

    this->setWindowTitle(n);
    ui->type_v->setText(ty);
    ui->pos_v->setText(p);
    ui->size_v->setText(s);
    ui->time_v->setText(t);
}

