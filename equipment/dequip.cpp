#include "dequip.h"
#include "ui_dequip.h"

Dequip::Dequip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dequip)
{
    peCount = 0;
    ui->setupUi(this);
    for (int i = 0; i < 24; i++){
        Finished[i] = false;
        newFinished[i] = false;
    }
    ui->tableWidget->setColumnCount(17);
    ui->tableWidget->setRowCount(24);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"进程id"<<"蓝牙需求数"<<"键盘需求数"<<"鼠标需求数"<<"打印机需求数"<<"蓝牙已得数"<<"键盘已得数"<<"鼠标已得数"<<"打印机已得数"<<"蓝牙仍需数"<<"键盘仍需数"<<"鼠标仍需数"<<"打印机仍需数"<<"蓝牙剩余数"<<"键盘剩余数"<<"鼠标剩余数"<<"打印机剩余数");
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    for (int i = 0; i < 24; i++){
        for (int j = 0; j < 17; j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(" "));
        }
    }
    for (int j = 1; j < 5; j++){
        for (int i = 0; i < 24; i++){
            ui->tableWidget->item(i,j)->setBackgroundColor(Qt::gray);
        }
    }
    for (int j = 5; j < 9; j++){
        for (int i = 0; i < 24; i++){
            ui->tableWidget->item(i,j)->setBackgroundColor(Qt::white);
        }
    }
    for (int j = 9; j < 13; j++){
        for (int i = 0; i < 24; i++){
            ui->tableWidget->item(i,j)->setBackgroundColor(Qt::gray);
        }
    }
    for (int j = 13; j < 17; j++){
        for (int i = 0; i < 24; i++){
            ui->tableWidget->item(i,j)->setBackgroundColor(Qt::white);
        }
    }

}

Dequip::~Dequip()
{
    delete ui;
}
void Dequip::compareSlot(int requestData[], int pId, int time){
    if (requestData[0] <= ui->tableWidget->item(pId,9)->text().toInt() && requestData[1] <= ui->tableWidget->item(pId,10)->text().toInt() && requestData[2] <= ui->tableWidget->item(pId,11)->text().toInt() && requestData[3] <= ui->tableWidget->item(pId,12)->text().toInt()){
        if (requestData[0] <= ui->tableWidget->item(0,13)->text().toInt() && requestData[1] <= ui->tableWidget->item(0,14)->text().toInt() && requestData[2] <= ui->tableWidget->item(0,15)->text().toInt() && requestData[3] <= ui->tableWidget->item(0,16)->text().toInt()){
            //执行安全性算法
            for (int i = 0; i < 4; i ++){
               ui->tableWidget->setItem(0, i+13, new QTableWidgetItem(QString::number(ui->tableWidget->item(0, i+13)->text().toInt()-requestData[i])));
            }
            if (security(pId)){
                QMessageBox::information(this, "提示", "安全性算法检测通过！");
                Finished[pId] = true;
                //更新剩余资源数
                for (int i = 0; i < 4; i++){
                    ui->tableWidget->setItem(pId, i+5, new QTableWidgetItem(QString::number(ui->tableWidget->item(pId, i+5)->text().toInt() + requestData[i])));
                    ui->tableWidget->setItem(pId, i+9, new QTableWidgetItem(QString::number(ui->tableWidget->item(pId, i+9)->text().toInt() - requestData[i])));
                }
                myTimerId = this->startTimer(time);
                pe[peCount].timeId = myTimerId;
                pe[peCount].pId = pId;
                peCount++;
                QMessageBox::information(this,"提示","请求占用资源成功，该进程占用资源"+QString::number(time)+"毫秒");
            }
            else{
                for (int i = 0; i < 4; i ++){
                   ui->tableWidget->setItem(0, i+13, new QTableWidgetItem(QString::number(ui->tableWidget->item(0, i+13)->text().toInt()+requestData[i])));
                }
                QMessageBox::warning(this, "警告", "安全性算法不通过，拒绝分配！");
                return;
            }
        }
        else{
            QMessageBox::warning(this,"警告","系统剩余资源数不足，进程阻塞！");
            emit sendWaitToWidget(pId);
        }
    }
    else{
        QMessageBox::warning(this,"警告","请求资源数大于所需资源数，拒绝请求！");
    }
}

void Dequip::on_requestButton_clicked()
{
    request *re = new request;
    re->show();
    connect(re, &request::sendToDequip, this,&Dequip::compareSlot);
}

void Dequip::timerEvent(QTimerEvent *event){
    for (int i = 0; i < 500; i++){
        if (event->timerId() == pe[i].timeId){
            for (int j = 0; j < 4; j++){
                ui->tableWidget->setItem(0, j+13, new QTableWidgetItem(QString::number(ui->tableWidget->item(0, j+13)->text().toInt()+ui->tableWidget->item(pe[i].pId, j+5)->text().toInt())));
                ui->tableWidget->setItem(pe[i].pId, j+5, new QTableWidgetItem("0"));
                ui->tableWidget->setItem(pe[i].pId, j+9, new QTableWidgetItem("0"));
            }
            QMessageBox::information(this, "提示", "进程占用资源结束，该进程占有的所有资源已归还");
            this->killTimer(pe[i].timeId);
        }
    }

}
  //安全性算法
bool Dequip::security(int pId){
    int oldData[4];
    for (int i = 0; i < 4; i++){
        oldData[i] = ui->tableWidget->item(0, i + 13)->text().toInt();
    }
    while(true){
        bool isSame = true;
        for (int i = 0; i < 24; i++){
            bool isOk = true;
            if (i == pId)continue;
            if (Finished[i] == true) continue;   //如果当前进程已分配，则继续循环
            for (int j = 0; j < 4; j++){
                if (ui->tableWidget->item(i, j + 9)->text().toInt() > ui->tableWidget->item(0, j + 13)->text().toInt()){
                    isOk = false;
                }
            }
            if (isOk == true){
                newFinished[i] = true;
                for (int j = 0; j < 4; j++){
                    ui->tableWidget->setItem(0, j+13, new QTableWidgetItem(QString::number(ui->tableWidget->item(i, j + 5)->text().toInt() + ui->tableWidget->item(0, j + 13)->text().toInt())));
                }
            }
        }
        for (int i = 0; i < 24; i++){
            if (Finished[i] != newFinished[i]){
                isSame = false;
                for (int j = 0; j < 24; j++){
                    Finished[j] = newFinished[j];
                }
            }
        }
        if (isSame == true){
            break;
        }
    }
    for (int i = 0; i < 4; i++){
        ui->tableWidget->setItem(0, i+13, new QTableWidgetItem(QString::number(oldData[i])));
    }
    for (int i = 0; i < 24; i++){
        if (i == pId) continue;
        if (Finished[i] == false){
            for (int j = 0; j < 24; j++){
                //qDebug() <<Finished[j];
            }
            return false;
        }
    }
    //qDebug()<<"yyy";
    return true;
}
