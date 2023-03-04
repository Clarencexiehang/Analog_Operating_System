#include "file.h"
#include "ui_file.h"

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);

    //初始化 文件信息 数组
    filepros=new QVector<myFilepro*>;
    //新建目录
    QTreeWidgetItem *rootItem1=new QTreeWidgetItem(ui->filetree);
    rootItem1->setText(0,"文件夹1");
    rootItem1->setIcon(0,QIcon(":/img/file\\myPhotos/dirIcon.png"));
    //目录加入
    myFilepro *fd=new myFilepro;
    QDateTime nd=QDateTime::currentDateTime();
    QString time = nd.toString("yyyy-MM-dd");
    fd->setData(filepros->length(),"文件夹",rootItem1->text(0),"pos","/",time,"默认");
    filepros->append(fd);

    //新建文件
    QTreeWidgetItem *file=new QTreeWidgetItem(rootItem1);
    file->setText(0,"文件1.txt");
    file->setIcon(0,QIcon(":/img/file\\myPhotos/file.png"));

    //文件加入数组
    myFilepro *f=new myFilepro;
    QDateTime nd1=QDateTime::currentDateTime();
    QString time1 = nd.toString("yyyy-MM-dd");
    f->setData(filepros->length(),"文件",file->text(0),"pos","1",time1,"默认");
    filepros->append(f);

    //新建目录
    QTreeWidgetItem *rootItem2=new QTreeWidgetItem(ui->filetree);
    rootItem2->setText(0,"文件夹2");
    rootItem2->setIcon(0,QIcon(":/img/file\\myPhotos/dirIcon.png"));
    //目录加入
    myFilepro *fd2=new myFilepro;
    QDateTime nd2=QDateTime::currentDateTime();
    QString time2 = nd.toString("yyyy-MM-dd");
    fd2->setData(filepros->length(),"文件夹",rootItem1->text(0),"pos","/",time2,"默认");
    filepros->append(fd2);
}

File::~File()
{
    delete ui;
}


void File::on_mag_createfile_clicked()
{
    //获取当前目录
    pCurrentItem=ui->filetree->currentItem();

    //创建“创建文件”窗口
    fcw=new fileCreate;
    fcw->setWindowTitle("创建文件");
    //信号、槽连接
    void (fileCreate::*sig)(QString,int,QString)=&fileCreate::createfile;
    void (File::*get)(QString,int,QString)=&File::getFilename;
    connect(fcw,sig,this,get);

    fcw->show();
}

void File::on_mag_createfoler_clicked()
{
    //获取当前目录
    pCurrentItem=ui->filetree->currentItem();
    //创建“创建文件夹”窗口
    flw=new folderCreate;
    flw->setWindowTitle("创建文件");
    //信号、槽连接
    void (folderCreate::*sig)(QString)=&folderCreate::createfolder;
    void (File::*get)(QString)=&File::getFoldername;
    connect(flw,sig,this,get);

    flw->show();
}

void File::getFilename(QString filename,int size,QString p)
{
    qDebug()<<"在 "<<pCurrentItem->text(0)<<"下创建文件";

    QTreeWidgetItem *file=new QTreeWidgetItem(pCurrentItem);
    file->setText(0,filename);
    file->setIcon(0,QIcon(":/img/file\\myPhotos/file.png"));

    //文件加入数组
    myFilepro *f=new myFilepro;
    QDateTime nd=QDateTime::currentDateTime();
    QString time = nd.toString("yyyy-MM-dd");
    f->setData(filepros->length(),"文件",file->text(0),"pos",QString::number(size),time,p);
    filepros->append(f);

    //调用磁盘空间分配函数   初始大小为 0 kB
    disk->distrcFileSpace(f->id,f->name,(f->size).toInt(),p);
}

void File::getFoldername(QString name)
{
    qDebug()<<"在 "<<pCurrentItem->text(0)<<"下创建文件夹";

    QTreeWidgetItem *file=new QTreeWidgetItem(pCurrentItem);
    file->setText(0,name);
    file->setIcon(0,QIcon(":/img/file\\myPhotos/dirIcon.png"));

    //文件夹加入数组
    myFilepro *fd=new myFilepro;
    QDateTime nd=QDateTime::currentDateTime();
    QString time = nd.toString("yyyy-MM-dd");
    fd->setData(filepros->length(),"文件夹",file->text(0),"pos","/",time,"默认");
    filepros->append(fd);
}

void File::on_filetree_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* curItem=ui->filetree->itemAt(pos);  //获取当前被点击的节点
    if(curItem==NULL)return;
    qDebug()<<"右击了 : "<<curItem->text(0);
    QVariant var = curItem->data(0,Qt::UserRole);

    //查找该文件的属性
    myFilepro *f=new myFilepro;
    QVector<myFilepro*>::iterator iter;
    for (iter=filepros->begin();iter!=filepros->end();iter++)
    {
        if(((*iter)->name)==curItem->text(0)){
            f->id=(*iter)->id;
            f->name=(*iter)->name;
            f->type=(*iter)->type;
            f->pos=(*iter)->pos;
            f->size=(*iter)->size;
            f->time=(*iter)->time;
            break;
        }
    }

    //创建属性窗口
    fileProperty *fp=new fileProperty;
    fp->setWindowTitle("文件属性");
    fp->move(QCursor::pos());
    //*****************************属性信号、槽连接
    //文件窗口(文件属性)->属性窗口
    void (File::*sig)(int,QString,QString,QString,QString,QString)=&File::propertyInfoTran;
    void (fileProperty::*pro)(int,QString,QString,QString,QString,QString)=&fileProperty::getFilepropertyInfo;
    connect(this,sig,fp,pro);
    //属性窗口(查看文件分配情况)->磁盘窗口
    void (fileProperty::*sig1)(int,QString,QString,QString,int,QString)=&fileProperty::diskLookFileSpace;
    void (Disk::*pro1)(int,QString,QString,QString,int,QString)=&Disk::LookFileSpace;
    connect(fp,sig1,this->disk,pro1);

    //信号触发
    emit this->propertyInfoTran(f->id,f->type,f->name,f->pos,f->size,f->time);

    this->fp=fp;
    fp->show();
}

void File::on_del_file_clicked()
{
    //获取当前目录
    pCurrentItem=ui->filetree->currentItem();
    qDebug()<<"删除 "<<pCurrentItem->text(0);
    //____________________ 删除磁盘空间
    //在文件记录中根据文件名找文件分配策略
    QString policy;
    QVector<myFilepro*>::iterator iter;
    for (iter=filepros->begin();iter!=filepros->end();iter++){
        if((*iter)->name==pCurrentItem->text(0)){
               policy=(*iter)->policy;
               break;
        }
    }
    this->disk->releaseSpace(pCurrentItem->text(0),policy);
    //____________________ 删除树结构
    delete pCurrentItem;
}

void File::on_filetree_doubleClicked(const QModelIndex &index)
{
    pCurrentItem=ui->filetree->currentItem();
    //判断双击的是否为文件
    bool isFile=false;
    for (int i=0;i<(this->disk->fpol).size();i++) {
        if((this->disk->fpol)[i].filename==pCurrentItem->text(0)){
            isFile=true;
        }
    }
    if(isFile){
        contextWind=new FileContext;
        contextWind->setFileName(pCurrentItem->text(0));
        contextWind->show();

        //获取当前目录
        pCurrentItem=ui->filetree->currentItem();
        qDebug()<<"打开文件 ： "<<pCurrentItem->text(0);
        //在文件记录中根据文件名找文件分配策略
        QString policy;
        QVector<myFilepro*>::iterator iter;
        for (iter=filepros->begin();iter!=filepros->end();iter++){
            if((*iter)->name==pCurrentItem->text(0)){
                   policy=(*iter)->policy;
                   break;
            }
        }
        this->disk->geentFileSeekWind(pCurrentItem->text(0),policy);
    }
}

QVector<myFilepro *> *File::getFileContext()
{
    return this->filepros;
}


