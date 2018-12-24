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
    void setX(int ptr, int value){
        pCodeTable[ptr].setX(value);
    }
    void setY(int ptr, int value){
        pCodeTable[ptr].setY(value);
    }
    void setOP(int ptr, PCodeOperator value){
        pCodeTable[ptr].setOP(value);
    }
private:
    QList<PerPCode> pCodeTable;
};

#endif // PCODETABLE_H
