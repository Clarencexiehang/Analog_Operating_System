#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QDateTime>

#include"disk/disk.h"
#include"filecreate.h"
#include"foldercreate.h"
#include"fileproperty.h"
#include"myfilepro.h"
#include"spaceexband.h"
#include"filecontext.h"

#include<QVector>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    Disk *disk;//磁盘对象
    fileProperty *fp;//文件属性

signals:
    //信号：磁盘中为新文件分配空间
    void getDiskMemery(int id,QString name,int size,QString sp);
    //信号：文件右键属性信息传送
    void propertyInfoTran(int i,QString ty,QString n,QString p,QString s,QString t);


public slots:
    void on_mag_createfile_clicked();

    void on_mag_createfoler_clicked();

    void getFilename(QString filename,int size,QString p);
    void getFoldername(QString name);

    void on_filetree_customContextMenuRequested(const QPoint &pos);

    void on_del_file_clicked();

    void on_filetree_doubleClicked(const QModelIndex &index);

    //返回文件信息键值对列表
    QVector<myFilepro*> *getFileContext();


private slots:
    void on_search_btn_clicked();

    void on_search_res_tree_doubleClicked(const QModelIndex &index);

private:
    Ui::File *ui;
    QTreeWidgetItem *pCurrentItem;
    fileCreate *fcw;
    folderCreate *flw;
    spaceExband *spaceExbanWind;//内存扩充窗口
    FileContext *contextWind;//文件内容窗口
    //文件信息键值对
    QVector<myFilepro*> *filepros;
};

#endif // FILE_H
