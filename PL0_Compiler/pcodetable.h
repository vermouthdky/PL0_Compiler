#ifndef PCODETABLE_H
#define PCODETABLE_H

#include<QList>
#include<perpcode.h>

class PCodeTable
{
public:
    PCodeTable();
    QList<PerPCode> getPCodeTable(){return pCodeTable;}
    int getPCodePtr(){return pCodeTable.size();}
    void addPerPCode(PerPCode item){pCodeTable.append(item);}
    void addPerPCode(PCodeOperator op, int x, int y){
        PerPCode item = PerPCode(op,x,y);
        pCodeTable.append(item);
    }
private:
    QList<PerPCode> pCodeTable;
};

#endif // PCODETABLE_H
