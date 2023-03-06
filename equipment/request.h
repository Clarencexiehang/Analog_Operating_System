#ifndef REQUEST_H
#define REQUEST_H

#include <QWidget>
#include <QMessageBox>
#include "dequip.h"
#include "ui_dequip.h"
#include "widget.h"
#include <QTableWidget>
namespace Ui {
class request;
}

class request : public QWidget
{
    Q_OBJECT

public:
    explicit request(QWidget *parent = 0);
    Ui::request *ui;
    ~request();
    int requestId;
    int requestBlue;
    int requestKey;
    int requestMouse;
    int requestPrint;
    int requestTime;
signals:
    void sendToDequip(int requestData[], int pId, int time);
private slots:
    void on_requestButton_clicked();
private:

};

#endif // REQUEST_H
