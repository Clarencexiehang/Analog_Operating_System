#ifndef FATFILESPACEDETAIL_H
#define FATFILESPACEDETAIL_H

#include <QWidget>
#include<QTableWidget>
#include<QTableWidgetItem>

namespace Ui {
class fatFileSpaceDetail;
}

class fatFileSpaceDetail : public QWidget
{
    Q_OBJECT

public:
    explicit fatFileSpaceDetail(QWidget *parent = nullptr);
    ~fatFileSpaceDetail();
    QTableWidget *getFatTableWidget();
    QTableWidget *getFatDirTableWidget();
private:
    Ui::fatFileSpaceDetail *ui;
};

#endif // FATFILESPACEDETAIL_H
