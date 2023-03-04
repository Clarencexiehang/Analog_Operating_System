#include "spaceexband.h"
#include "ui_spaceexband.h"

spaceExband::spaceExband(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spaceExband)
{
    ui->setupUi(this);
    ui->exband_size->setPlaceholderText("输入扩展大小");
}

spaceExband::~spaceExband()
{
    delete ui;
}

void spaceExband::setData(QString filename,QString policy)
{
    this->filename=filename;
    this->policy=policy;
    ui->filename_lab->setText(this->filename);
}

void spaceExband::on_btn_exband_clicked()
{
    QString exbandType=ui->exband_type->currentText();
    QString exbandSize=ui->exband_size->text();

    emit this->fileExband(this->policy,this->filename,exbandType,exbandSize);
    this->close();
}
