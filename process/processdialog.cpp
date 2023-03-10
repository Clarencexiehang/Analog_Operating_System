#include "processdialog.h"
#include "ui_processdialog.h"
#include "QMessageBox"
#include "QDebug"
#include "mainwindow.h"
#include "pcb.h"

processDialog::processDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::processDialog)
{
    ui->setupUi(this);

    ui->lineEdit_proid->setPlaceholderText("请输入标识符");
    ui->lineEdit_prio->setPlaceholderText("请输入优先级");
    ui->lineEdit_cpuTime->setPlaceholderText("请输入所需CPU时间");

}

processDialog::~processDialog()
{
    delete ui;
}

void processDialog::on_pushButton_ok_clicked()
{
    if(ui->lineEdit_proid->text().isEmpty()||ui->lineEdit_prio->text().isEmpty()||ui->lineEdit_cpuTime->text().isEmpty()){
        QMessageBox::critical(this,"错误提醒",tr("请完成数据输入"));
        return;
    }

    QString name = ui->lineEdit_proid->text();
    QString prio = ui->lineEdit_prio->text();
    QString cpuTime = ui->lineEdit_cpuTime->text();

    //存入readyQueue中
    PCB* process = new PCB(name.toStdString());
    process->needTime = cpuTime.toInt();
    process->prio = prio.toInt();

    if(process->needTime<=0||process->needTime>20){
        QMessageBox::critical(this,"格式错误",tr("输入值必须在[0,20]内"));
        return ;
    }
    if(process->prio<=0||process->prio>20){
        QMessageBox::critical(this,"格式错误",tr("输入值必须在[0,20]内"));
        return ;
    }

    //放入就绪队列
    extern MainWindow *w;
    w->processTab->processQueue.push_back(process);
    w->processTab->readyQueue.push_back(process);


    //更新表格
    w->processTab->showProcess();
    this->close();
}

void processDialog::on_pushButton_cancel_clicked()
{
    this->close();
}
