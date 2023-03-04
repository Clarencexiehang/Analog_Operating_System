#ifndef SHOWFILESEEKODER_H
#define SHOWFILESEEKODER_H

#include <QWidget>
#include<QVector>
#include<QtDebug>
namespace Ui {
class showFileSeekOder;
}

class showFileSeekOder : public QWidget
{
    Q_OBJECT

public:
    explicit showFileSeekOder(QWidget *parent = nullptr);
    ~showFileSeekOder();
    void fileDispatch(QVector<int> blockCyIdOrder);
private:
    Ui::showFileSeekOder *ui;
};

#endif // SHOWFILESEEKODER_H
