#ifndef CONFILESPACEDETAIL_H
#define CONFILESPACEDETAIL_H

#include <QWidget>
#include<QTableWidget>
#include<QTableWidgetItem>


namespace Ui {
class conFileSpaceDetail;
}

class conFileSpaceDetail : public QWidget
{
    Q_OBJECT

public:
    explicit conFileSpaceDetail(QWidget *parent = nullptr);
    ~conFileSpaceDetail();
    QTableWidget *getconfTableWidget();
    QTableWidget *getconfDirTableWidget();

private:
    Ui::conFileSpaceDetail *ui;
};

#endif // CONFILESPACEDETAIL_H
