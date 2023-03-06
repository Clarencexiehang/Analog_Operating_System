#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process/processtab.h"
#include "memory/memory.h"
#include "equipment/Widget.h"
#include "file/file.h"
#include "disk/disk.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ProcessTab *processTab;
    Memory *memoryTab;
    File *fileTab;
    Widget *equipmentTab;
    Disk *diskTab;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
