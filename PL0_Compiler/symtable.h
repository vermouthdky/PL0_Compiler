#ifndef SYMTABLE_H
#define SYMTABLE_H

#include<persym.h>
#include<QList>
#include<QString>

class SymTable
{
public:
    SymTable();
    void enterVar(QString name, int level, int address);
    void enterConst(QString name, int level, int value, int address);
    void enterProcess(QString name, int level, int address);
    bool thisLayerExisted(QString name, int level);
    bool preLayerExisted(QString name, int level);
    PerSym& getSymbol(QString name);
    int getLevelProc(int level);
    void clearLayer(int level);

    QList<PerSym>& getSymbolTable(){return symbolTable;}
    void setPtr(int ptr){this->ptr = ptr;}
    int getPtr(){return ptr;}
    int getLength(){return symbolTable.size();}
    int getCon(){return con;}
    int getVar(){return var;}
    int getProc(){return proc;}


private:
    const int con = 1;
    const int var = 2;
    const int proc = 3;
    int ptr = 0;
    QList <PerSym> symbolTable;


};

#endif // SYMTABLE_H
