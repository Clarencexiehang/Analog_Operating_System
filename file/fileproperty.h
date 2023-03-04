#ifndef FILEPROPERTY_H
#define FILEPROPERTY_H

#include <QWidget>
#include<QDebug>

namespace Ui {
class fileProperty;
}

class fileProperty : public QWidget
{
    Q_OBJECT

public:
    explicit fileProperty(QWidget *parent = nullptr);
    ~fileProperty();

    int id;
    QString type;
    QString name;
    QString pos;
    QString size;
    QString time;

//信号
signals:
    //查看某一文件的磁盘分配情况
    void diskLookFileSpace(int id,QString name,QString type,QString pos,int size,QString time);

//槽：
public slots:
    void on_btn_close_clicked();

    void on_size_detail_clicked();

    //获取文件属性信息
    void getFilepropertyInfo(int i,QString ty,QString n,QString p,QString s,QString t);


private:
    Ui::fileProperty *ui;
};

#endif // FILEPROPERTY_H
