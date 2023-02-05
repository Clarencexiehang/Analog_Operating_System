#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>

namespace Ui {
class processDialog;
}

class processDialog : public QDialog
{
    Q_OBJECT

public:
    explicit processDialog(QWidget *parent = nullptr);
    ~processDialog();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

signals:                        //信号不能像普通函数一样使用，也不需要权限声明
    void sendToShowProcess();   //信号不需要定义

private:
    Ui::processDialog *ui;
    void *mainWindowPtr;
};

#endif // PROCESSDIALOG_H
