#include "filecreate.h"
#include "ui_filecreate.h"


fileCreate::fileCreate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileCreate)
{
    ui->setupUi(this);
    ui->filename->setPlaceholderText("请输入文件名");
    ui->need_size->setText(QString::number(10));
    ui->need_size->setEnabled(false);
}

fileCreate::~fileCreate()
{
    delete ui;
}

void fileCreate::on_createfile_clicked()
{
    if(ui->filename->text().isEmpty()){
        QMessageBox::critical(this,"错误提醒",tr("文件名不能为空 ！"));
    }else{
        //信号触发
        QString size=ui->need_size->text();
        emit this->createfile(ui->filename->text(),size.toInt(),ui->space_policy->currentText());
        this->close();
    }
}

void fileCreate::on_cancel_createfile_clicked()
{
    qDebug()<<"取消 创建文件 ......";
    this->close();
}
