#include "symtable.h"

SymTable::SymTable()
{

}

void SymTable::enterConst(QString name, int level, int value, int address){
    PerSym newitem = PerSym(con, value, level, address, 0, name);
    symbolTable.append(newitem);
    ptr++;
}
void SymTable::enterVar(QString name, int level, int address){
    PerSym newitem = PerSym(var, level, address, 0, name);
    symbolTable.append(newitem);
    ptr++;
}
void SymTable::enterProcess(QString name, int level, int address){
    PerSym newitem = PerSym(proc, level, address, 0, name);
    symbolTable.append(newitem);
    ptr++;
}

// do have some problems for thislayerExisted and preLayerExited
bool SymTable::thisLayerExisted(QString name, int level){
    for(int i=0;i<symbolTable.size();i++){
        if(!symbolTable[i].getName().compare(name) && symbolTable[i].getLevel()==level){
            return true;
        }
    }
    return false;
}
// do have some problems for thislayerExisted and preLayerExited
bool SymTable::preLayerExisted(QString name, int level){
    for(int i=0;i<symbolTable.size();i++){
        if(!symbolTable[i].getName().compare(name) && symbolTable[i].getLevel()<=level){
            return true;
        }
    }
    return false;
}
PerSym& SymTable::getSymbol(QString name){
    for(int i=symbolTable.size()-1; i>=0;i--){
        if(!symbolTable[i].getName().compare(name)){
            return symbolTable[i];
        }
    }
}

void SymTable::clearLayer(int level){
    for(int i=symbolTable.size()-1;i>=0;i--){
        if(symbolTable[i].getLevel() == level){
            symbolTable.removeAt(i);
        }
    }
}

int SymTable::getLevelProc(int level){
    for(int i=symbolTable.size()-1;i>=0;i--){
        if(symbolTable[i].getType()==proc && symbolTable[i].getLevel()==level){
            return i;
        }
    }
    return -1;
}

