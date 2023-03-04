#ifndef FILECONTEXT_H
#define FILECONTEXT_H
#include <QWidget>
#include"myfilepro.h"

namespace Ui {
class FileContext;
}

class FileContext : public QWidget
{
    Q_OBJECT

public:
    explicit FileContext(QWidget *parent = nullptr);
    ~FileContext();
    QString filename;
    void setFileName(QString filename);

private slots:
    void on_save_clicked();

    void on_close_clicked();

private:
    Ui::FileContext *ui;
};

#endif // FILECONTEXT_H
