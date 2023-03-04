#include "myfilepro.h"

myFilepro::myFilepro()
{

}

void myFilepro::setData(int i,QString ty,QString n, QString p, QString s, QString t,QString po)
{
    this->id=i;
    this->type=ty;
    this->name=n;
    this->pos=p;
    this->size=s;
    this->time=t;
    this->policy=po;
}
