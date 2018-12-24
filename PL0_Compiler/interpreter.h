#ifndef INTERPRETER_H
#define INTERPRETER_H
#include<QList>
#include<perpcode.h>
#include<pcodetable.h>

class interpreter
{
public:
    interpreter();
    interpreter(QList<int> input);
    void setPcodeTable(PCodeTable pcodetable);
    QStringList getOutput();
    QString run();


private:
    static const int stackSize = 5000;
    int stack[stackSize];
    QList<PerPCode> pcode;
    QList<int> input;
    int inputPtr;
    QStringList output;

    int pc;
    int base;
    int top;

    int getBase(int BP, int lev);

};

#endif // INTERPRETER_H
