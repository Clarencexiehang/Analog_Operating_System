#include "filecontext.h"
#include "ui_filecontext.h"
#include "mainwindow.h"

extern MainWindow *w;

FileContext::FileContext(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileContext)
{
    ui->setupUi(this);

}

FileContext::~FileContext()
{
    delete ui;
}

void FileContext::setFileName(QString filename)
{
    this->filename=filename;

    QVector<myFilepro*> *filepros=w->fileTab->getFileContext();
    for (int i=0;i<filepros->size();i++) {
        if(((*filepros)[i])->name==this->filename){
            //改变文件内容
            ui->context->setText(((*filepros)[i])->context);
            break;
        };
    }
}

void FileContext::on_save_clicked()
{
    QVector<myFilepro*> *filepros=w->fileTab->getFileContext();
    QString s_con_str;//初始大小
    QString e_con_str;//改变后大小
    myFilepro mf;
    qDebug()<<"查找文件 ："<<this->filename;
    for (int i=0;i<filepros->size();i++) {
        qDebug()<<"遍历文件 ："<<((*filepros)[i])->name;
        if(((*filepros)[i])->name==this->filename){
            qDebug()<<"找到文件 ："<<((*filepros)[i])->name;
            s_con_str=((*filepros)[i])->context;
            e_con_str=ui->context->toPlainText();
            //改变文件内容
            ((*filepros)[i])->context=ui->context->toPlainText();

            mf.policy=((*filepros)[i])->policy;
            mf.name=((*filepros)[i])->name;
            mf.type=((*filepros)[i])->type;

            break;
        };

    }
    int s_size=s_con_str.size();
    int e_size=e_con_str.size();
    QString expanType=(e_size-s_size)>=0?"扩展":"缩小";
    QString exSize=QString::number(e_size-s_size);
    qDebug()<<"初始大小 ："<<s_size<<" 最终大小："<<e_size;

    w->fileTab->disk->fileExband(mf.policy,mf.name,expanType,exSize);
}

void FileContext::on_close_clicked()
{
    this->close();
}
