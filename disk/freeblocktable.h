#ifndef FREEBLOCKTABLE_H
#define FREEBLOCKTABLE_H

#include <QWidget>
#include<QTableWidget>
#include<QTableWidgetItem>

namespace Ui {
class freeBlockTable;
}

class freeBlockTable : public QWidget
{
    Q_OBJECT

public:
    explicit freeBlockTable(QWidget *parent = nullptr);
    ~freeBlockTable();
    QTableWidget *getFreeBlockTable();
private:
    Ui::freeBlockTable *ui;
};

#endif // FREEBLOCKTABLE_H
