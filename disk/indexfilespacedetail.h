#ifndef INDEXFILESPACEDETAIL_H
#define INDEXFILESPACEDETAIL_H

#include <QWidget>
#include<QTableWidget>
#include<QTableWidgetItem>

namespace Ui {
class indexFileSpaceDetail;
}

class indexFileSpaceDetail : public QWidget
{
    Q_OBJECT

public:
    explicit indexFileSpaceDetail(QWidget *parent = nullptr);
    ~indexFileSpaceDetail();
    QTableWidget *getindexFileTable();
    QTableWidget *getindexFileDirTable();
private:
    Ui::indexFileSpaceDetail *ui;
};

#endif // INDEXFILESPACEDETAIL_H
