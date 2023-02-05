#ifndef FILE_H
#define FILE_H

#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();

private:
    Ui::File *ui;
};

#endif // FILE_H
