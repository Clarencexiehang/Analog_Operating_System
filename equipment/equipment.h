#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QWidget>

namespace Ui {
class Equipment;
}

class Equipment : public QWidget
{
    Q_OBJECT

public:
    explicit Equipment(QWidget *parent = nullptr);
    ~Equipment();

private:
    Ui::Equipment *ui;
};

#endif // EQUIPMENT_H
