#ifndef TOKEN_H
#define TOKEN_H

#include<QString>
#include<Symbol.h>

class Token
{
private:
    Symbol sy;
    int line;
    QString value;


public:
    Token();
    Token(Symbol sy, int line, QString value){
        this->sy = sy;
        this->line = line;
        this->value = value;
    }
    void setSy(Symbol sy){this->sy = sy;}
    void setLine(int line){this->line = line;}
    void setValue(QString value){this->value = value;}
    Symbol getSy(){return sy;}
    int getLine(){return line;}
    QString getValue(){return value;}
};

#endif // TOKEN_H
