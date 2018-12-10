#ifndef PERSYM_H
#define PERSYM_H

#include<QString>


class PerSym
{
public:
    PerSym();
    PerSym(int type, int value, int level, int address, int size, QString name){
        this->type = type;
        this->value = value;
        this->level = level;
        this->address = address;
        this->size = size;
        this->name = name;
    }
    PerSym(int type, int level, int address, int size, QString name){
        // for declaration of variable or procedure
        // this is no initial value
        this->type = type;
        this->level = level;
        this->address = address;
        this->size = size;
        this->name = name;
    }

    bool isEmpty(){
        if(this->name.compare("")) return false;
        return true;
    }

    void setType(int _type) {
            type = _type;
        }

    void setValue(int _value) {
        value = _value;
    }

    void setLevel(int _level) {
        level = _level;
    }

    void setAddress(int _address) {
        address = _address;
    }

    void setSize(int _size) {
        size = _size;
    }

    void setName(QString _name) {
        name = _name;
    }

    int  getType() {
        return type;
    }

    int getValue() {
        return value;
    }

    int getLevel() {
        return level;
    }

    int getAddress() {
        return address;
    }

    int getSize() {
        return size;
    }

    QString getName() {
        return name;
    }


private:
    int type;
    int value;
    int level;
    int address;
    int size;
    QString name = "";
};

#endif // PERSYM_H
