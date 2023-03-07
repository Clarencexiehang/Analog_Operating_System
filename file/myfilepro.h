#ifndef MYFILEPRO_H
#define MYFILEPRO_H

#include<QWidget>

class myFilepro
{
public:
    myFilepro();
    void setData(int i,QString ty,QString n,QString p,QString s,QString t,QString po);

    int id;
    QString type;
    QString name;
    QString pos;
    QString size;
    int blockNum=0;
    QString time;
    QString policy;
    //内容
    QString context;
};

Q_DECLARE_METATYPE(myFilepro);

QDebug operator<<(QDebug dbg, const myFilepro &fp);

inline bool operator<(const myFilepro &fp1,const myFilepro &fp2){
    if(fp1.id!=fp2.id){
        return fp1.id<fp2.id;
    }else{
        return fp1.id>fp2.id;
    }
}

#endif // MYFILEPRO_H
