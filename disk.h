#ifndef DISK_H
#define DISK_H

#include <QWidget>

namespace Ui {
class Disk;
}

class Disk : public QWidget
{
    Q_OBJECT

public:
    explicit Disk(QWidget *parent = nullptr);
    ~Disk();

private:
    Ui::Disk *ui;
};

#endif // DISK_H
