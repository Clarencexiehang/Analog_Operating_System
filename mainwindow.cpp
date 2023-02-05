#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(850,666);
    setWindowTitle("模拟操作系统");

    //添加每个管理页面
    QTabWidget *tabWidget = new QTabWidget;
    processTab=new ProcessTab(this);
    memoryTab=new Memory(this);
    fileTab = new File(this);
    equipmentTab = new Equipment(this);

    tabWidget->addTab(processTab,"进程管理");
    tabWidget->addTab(memoryTab,"内存管理");
    tabWidget->addTab(fileTab,"文件管理");
    tabWidget->addTab(equipmentTab,"设备管理");
    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

