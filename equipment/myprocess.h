#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QWidget>

namespace Ui {
class myProcess;
}

class myProcess : public QWidget
{
    Q_OBJECT

public:
    explicit myProcess(QWidget *parent = 0);
    Ui::myProcess *ui;
    ~myProcess();

private slots:
    void on_backbutton_clicked();
private:

};

#endif // MYPROCESS_H
