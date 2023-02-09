#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1500,900);
    setWindowTitle("模拟操作系统");
    //添加每个管理页面
    processTab=new ProcessTab(this);
    memoryTab=new Memory(this);
    fileTab = new File(this);
    equipmentTab = new Equipment(this);
    diskTab = new Disk(this);
    ui->processArea->setWidget(processTab);
    ui->memoryArea->setWidget(memoryTab);
    ui->fileArea->setWidget(fileTab);
    ui->equipArea->setWidget(equipmentTab);
    ui->diskArea->setWidget(diskTab);

    //随机产生进程并展示
    srand(time(nullptr));
    this->processTab->Random_Create_PCB();
    this->processTab->showProcess();
}

MainWindow::~MainWindow()
{
    delete ui;
}

