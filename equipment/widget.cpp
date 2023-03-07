#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    srand((unsigned)time(NULL));
    ui->setupUi(this);
    totalBluetooth = 200;
    totalKeyboard = 200;
    totalMouse = 200;
    totalPrint = 200;
    reBlue = 200;
    reKey = 200;
    reMouse = 200;
    rePrint = 200;
    for (int i = 0; i < 24; i++){
        proc[i].getBluetooth = 0;
        proc[i].getKey = 0;
        proc[i].getMouse = 0;
        proc[i].getPrint = 0;
    }
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(80);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"进程序列"<<"蓝牙资源数"<<"键盘资源数"<<"鼠标资源数"<<"打印机资源数");
    ui->tableWidget->setRowCount(24);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_createPro_clicked()
{
    for (int i = 0; i < 24; i++){
        proc[i].pId = i;
        proc[i].bluetooth = rand() % 20;
        proc[i].getBluetooth = rand() % 5;
        while (proc[i].getBluetooth > proc[i].bluetooth){
            proc[i].getBluetooth = rand() % 5;
        }
        proc[i].keyboard = rand() % 20;
        proc[i].getKey = rand() % 5;
        while (proc[i].getKey > proc[i].keyboard){
            proc[i].getKey = rand() % 5;
        }
        proc[i].mouse = rand() % 20;
        proc[i].getMouse = rand() % 5;
        while (proc[i].getMouse > proc[i].mouse){
            proc[i].getMouse = rand() % 5;
        }
        proc[i].print = rand() % 20;
        proc[i].getPrint = rand() % 5;
        while (proc[i].getPrint > proc[i].print){
            proc[i].getPrint = rand() % 5;
        }
        //qDebug() << proc[i].bluetooth;
    }
    QMessageBox::information(this, "提示", "生成成功！");
    for (int i = 0; i < 24; i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("p"+QString::number(proc[i].pId)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(proc[i].bluetooth)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(proc[i].keyboard)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(proc[i].mouse)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(proc[i].print)));
    }

}

void Widget::on_closeButton_clicked()
{
    Widget::close();
}


void Widget::on_pro1_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number(proc[0].pId));
    p->ui->blueEdit->setText(QString::number(proc[0].bluetooth));
    p->ui->keyEdit->setText(QString::number(proc[0].keyboard));
    p->ui->mouseEdit->setText(QString::number(proc[0].mouse));
    p->ui->printEdit->setText(QString::number(proc[0].print));
}

void Widget::on_pro2_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number(proc[1].pId));
    p->ui->blueEdit->setText(QString::number(proc[1].bluetooth));
    p->ui->keyEdit->setText(QString::number(proc[1].keyboard));
    p->ui->mouseEdit->setText(QString::number(proc[1].mouse));
    p->ui->printEdit->setText(QString::number(proc[1].print));


}

void Widget::on_pro3_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number(proc[2].pId));
    p->ui->keyEdit->setText(QString::number(proc[2].keyboard));
    p->ui->blueEdit->setText(QString::number(proc[2].bluetooth));
    p->ui->mouseEdit->setText(QString::number(proc[2].mouse));
    p->ui->printEdit->setText(QString::number(proc[2].print));
}

void Widget::on_pro4_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[3].pId));
    p->ui->keyEdit->setText(QString::number   (proc[3].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[3].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[3].mouse));
    p->ui->printEdit->setText(QString::number (proc[3].print));
}

void Widget::on_pro5_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[4].pId));
    p->ui->keyEdit->setText(QString::number   (proc[4].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[4].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[4].mouse));
    p->ui->printEdit->setText(QString::number (proc[4].print));

}

void Widget::on_pro6_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[5].pId));
    p->ui->keyEdit->setText(QString::number   (proc[5].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[5].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[5].mouse));
    p->ui->printEdit->setText(QString::number (proc[5].print));

}

void Widget::on_pro7_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[6].pId));
    p->ui->keyEdit->setText(QString::number   (proc[6].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[6].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[6].mouse));
    p->ui->printEdit->setText(QString::number (proc[6].print));

}

void Widget::on_pro8_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[7].pId));
    p->ui->keyEdit->setText(QString::number   (proc[7].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[7].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[7].mouse));
    p->ui->printEdit->setText(QString::number (proc[7].print));
}

void Widget::on_pro9_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[8].pId));
    p->ui->keyEdit->setText(QString::number   (proc[8].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[8].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[8].mouse));
    p->ui->printEdit->setText(QString::number (proc[8].print));
}

void Widget::on_pro10_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[9].pId));
    p->ui->keyEdit->setText(QString::number   (proc[9].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[9].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[9].mouse));
    p->ui->printEdit->setText(QString::number (proc[9].print));
}

void Widget::on_pro11_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[10].pId));
    p->ui->keyEdit->setText(QString::number   (proc[10].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[10].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[10].mouse));
    p->ui->printEdit->setText(QString::number (proc[10].print));
}

void Widget::on_pro12_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[11].pId));
    p->ui->keyEdit->setText(QString::number   (proc[11].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[11].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[11].mouse));
    p->ui->printEdit->setText(QString::number (proc[11].print));
}

void Widget::on_pro13_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[12].pId));
    p->ui->keyEdit->setText(QString::number   (proc[12].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[12].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[12].mouse));
    p->ui->printEdit->setText(QString::number (proc[12].print));
}

void Widget::on_pro14_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[13].pId));
    p->ui->keyEdit->setText(QString::number   (proc[13].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[13].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[13].mouse));
    p->ui->printEdit->setText(QString::number (proc[13].print));
}

void Widget::on_pro15_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[14].pId));
    p->ui->keyEdit->setText(QString::number   (proc[14].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[14].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[14].mouse));
    p->ui->printEdit->setText(QString::number (proc[14].print));
}

void Widget::on_pro16_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[15].pId));
    p->ui->keyEdit->setText(QString::number   (proc[15].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[15].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[15].mouse));
    p->ui->printEdit->setText(QString::number (proc[15].print));
}

void Widget::on_pro17_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[16].pId));
    p->ui->keyEdit->setText(QString::number   (proc[16].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[16].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[16].mouse));
    p->ui->printEdit->setText(QString::number (proc[16].print));
}

void Widget::on_pro18_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[17].pId));
    p->ui->keyEdit->setText(QString::number   (proc[17].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[17].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[17].mouse));
    p->ui->printEdit->setText(QString::number (proc[17].print));
}

void Widget::on_pro19_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[18].pId));
    p->ui->keyEdit->setText(QString::number   (proc[18].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[18].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[18].mouse));
    p->ui->printEdit->setText(QString::number (proc[18].print));
}

void Widget::on_pro20_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[19].pId));
    p->ui->keyEdit->setText(QString::number   (proc[19].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[19].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[19].mouse));
    p->ui->printEdit->setText(QString::number (proc[19].print));
}

void Widget::on_pro21_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[20].pId));
    p->ui->keyEdit->setText(QString::number   (proc[20].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[20].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[20].mouse));
    p->ui->printEdit->setText(QString::number (proc[20].print));
}

void Widget::on_pro22_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[21].pId));
    p->ui->keyEdit->setText(QString::number   (proc[21].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[21].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[21].mouse));
    p->ui->printEdit->setText(QString::number (proc[21].print));
}

void Widget::on_pro23_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[22].pId));
    p->ui->keyEdit->setText(QString::number   (proc[22].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[22].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[22].mouse));
    p->ui->printEdit->setText(QString::number (proc[22].print));
}

void Widget::on_pro24_clicked()
{
    myProcess *p = new myProcess;
    p->show();
    p->ui->pIdEdit->setText("p"+QString::number   (proc[23].pId));
    p->ui->keyEdit->setText(QString::number   (proc[23].keyboard));
    p->ui->blueEdit->setText(QString::number  (proc[23].bluetooth));
    p->ui->mouseEdit->setText(QString::number (proc[23].mouse));
    p->ui->printEdit->setText(QString::number (proc[23].print));
}

void Widget::waitAppendSlot(int pId){
    wait.enqueue(proc[pId]);
}

void Widget::on_StartButton_clicked()
{
    Dequip *dist = new Dequip;
    dist->show();
    for (int i = 0; i < 24; i++){
        dist->ui->tableWidget->setItem(i,0,new QTableWidgetItem("p"+QString::number(proc[i].pId)));
        dist->ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(proc[i].bluetooth)));
        dist->ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(proc[i].keyboard)));
        dist->ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(proc[i].mouse)));
        dist->ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(proc[i].print)));
        dist->ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(proc[i].getBluetooth)));
        dist->ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(proc[i].getKey)));
        dist->ui->tableWidget->setItem(i,7,new QTableWidgetItem(QString::number(proc[i].getMouse)));
        dist->ui->tableWidget->setItem(i,8,new QTableWidgetItem(QString::number(proc[i].getPrint)));
        dist->ui->tableWidget->setItem(i,9,new QTableWidgetItem(QString::number(proc[i].bluetooth-proc[i].getBluetooth)));
        dist->ui->tableWidget->setItem(i,10,new QTableWidgetItem(QString::number(proc[i].keyboard-proc[i].getKey)));
        dist->ui->tableWidget->setItem(i,11,new QTableWidgetItem(QString::number(proc[i].mouse-proc[i].getMouse)));
        dist->ui->tableWidget->setItem(i,12,new QTableWidgetItem(QString::number(proc[i].print-proc[i].getPrint)));
        int useBlue = 0;
        int useKey = 0;
        int useMouse = 0;
        int usePrint = 0;
        for (int i = 0; i < 24; i++){
            useBlue += proc[i].getBluetooth;
            useKey += proc[i].getKey;
            useMouse += proc[i].getMouse;
            usePrint += proc[i].getPrint;
        }
        reBlue = totalBluetooth - useBlue;
        reKey = totalKeyboard - useKey;
        reMouse = totalMouse - useMouse;
        rePrint = totalPrint - usePrint;
    }
    dist->ui->tableWidget->setItem(0,13,new QTableWidgetItem(QString::number(reBlue)));
    dist->ui->tableWidget->setItem(0,14,new QTableWidgetItem(QString::number(reKey)));
    dist->ui->tableWidget->setItem(0,15,new QTableWidgetItem(QString::number(reMouse)));
    dist->ui->tableWidget->setItem(0,16,new QTableWidgetItem(QString::number(rePrint)));
    connect(dist, &Dequip::sendWaitToWidget, this, &Widget::waitAppendSlot);

}
