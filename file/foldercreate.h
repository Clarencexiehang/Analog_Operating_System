#ifndef FOLDERCREATE_H
#define FOLDERCREATE_H

#include <QWidget>
#include<QtDebug>
#include<QMessageBox>


namespace Ui {
class folderCreate;
}

class folderCreate : public QWidget
{
    Q_OBJECT

public:
    explicit folderCreate(QWidget *parent = nullptr);
    ~folderCreate();

signals:
    void createfolder(QString fname);

private slots:
    void on_folder_create_clicked();

    void on_can_folder_create_clicked();

private:
    Ui::folderCreate *ui;
};

#endif // FOLDERCREATE_H
