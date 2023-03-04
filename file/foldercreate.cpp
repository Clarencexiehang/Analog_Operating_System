#include "foldercreate.h"
#include "ui_foldercreate.h"

folderCreate::folderCreate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::folderCreate)
{
    ui->setupUi(this);
}

folderCreate::~folderCreate()
{
    delete ui;
}

void folderCreate::on_folder_create_clicked()
{
    if(ui->foldername->text().isEmpty()){
        QMessageBox::critical(this,"错误提醒",tr("文件名不能为空 ！"));
    }else{
        //信号触发
        emit this->createfolder(ui->foldername->text());
        this->close();
    }
}

void folderCreate::on_can_folder_create_clicked()
{
    qDebug()<<"取消创建文件夹";
    this->close();
}
