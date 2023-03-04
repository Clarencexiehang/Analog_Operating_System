#ifndef SPACEEXBAND_H
#define SPACEEXBAND_H

#include <QWidget>
#include<QDebug>

namespace Ui {
class spaceExband;
}

class spaceExband : public QWidget
{
    Q_OBJECT

public:
    explicit spaceExband(QWidget *parent = nullptr);
    ~spaceExband();
    QString policy;
    QString filename;
    void setData(QString filename,QString policy);
signals:
    //文件扩充信号
    void fileExband(QString policy,QString filename,QString exbandType,QString exbanSize);

private slots:
    void on_btn_exband_clicked();

private:
    Ui::spaceExband *ui;
};

#endif // SPACEEXBAND_H
