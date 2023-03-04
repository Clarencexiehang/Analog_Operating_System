#ifndef FILECREATE_H
#define FILECREATE_H

#include <QWidget>
#include<QDebug>
#include<QMessageBox>
#include<QTreeWidgetItem>

namespace Ui {
class fileCreate;
}

class fileCreate : public QWidget
{
    Q_OBJECT

public:
    explicit fileCreate(QWidget *parent = nullptr);
    ~fileCreate();

signals:
    void createfile(QString filename,int size,QString sp);

private slots:
    void on_createfile_clicked();

    void on_cancel_createfile_clicked();

private:
    Ui::fileCreate *ui;
};

#endif // FILECREATE_H
