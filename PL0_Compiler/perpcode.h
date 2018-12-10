#ifndef PERPCODE_H
#define PERPCODE_H

#include<pcodeoperator.h>

class PerPCode
{
private:
    PCodeOperator OP;
    int X;
    int Y;
public:
    PerPCode(PCodeOperator OP, int X, int Y){
        this->OP = OP;
        this->X = X;
        this->Y = Y;
    }
    void setOP(PCodeOperator OP){this->OP = OP;}
    void setX(int X){this->X = X;}
    void setY(int Y){this->Y = Y;}
    PCodeOperator getOP(){return OP;}
    int getX(){return X;}
    int getY(){return Y;}

};

#endif // PERPCODE_H
