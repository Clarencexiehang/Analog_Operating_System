#ifndef DEQUIP_H
#define DEQUIP_H

#include <QWidget>
#include "request.h"
#include <ui_request.h>
#include <QTableWidget>
namespace Ui {
class Dequip;
}

class Dequip : public QWidget
{
    Q_OBJECT

public:
    explicit Dequip(QWidget *parent = 0);
    Ui::Dequip *ui;
    virtual void timerEvent(QTimerEvent *event);
    ~Dequip();
    bool security(int pId);
    bool Finished[24];
    bool newFinished[24];
signals:
    void sendWaitToWidget(int pId);
private slots:
    void on_requestButton_clicked();
    void compareSlot(int requestData[], int pId, int time);

private:
    int myTimerId;
    struct Pevent{
        int timeId;
        int pId;
    }pe[500];
    int peCount;
};

#endif // DEQUIP_H
