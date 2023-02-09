#ifndef MYPROCESSICON_H
#define MYPROCESSICON_H

#include <QWidget>

namespace Ui {
class MyProcessIcon;
}

class MyProcessIcon : public QWidget
{
    Q_OBJECT

public:
    explicit MyProcessIcon(QWidget *parent = nullptr);
    ~MyProcessIcon();

private:
    Ui::MyProcessIcon *ui;
};

#endif // MYPROCESSICON_H
