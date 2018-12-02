#ifndef LEXANY_H
#define LEXANY_H
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <cmath>
#include <QMainWindow>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QStack>
#include <utility>


class lexany
{
public:
    lexany(QString);
    lexany();

private:
    int strptr;
    QString input;
    QChar cur;
    QString token;
    int num;
    double dnum;
    enum Symbol {NONESY, BEGINSY, ENDSY, IFSY,
        THENSY, ELSESY, IDSY, INTSY, DOUBLESY,
        PLUSSY, MINUSSY, STARSY, DIVISY,
        LPARSY, RPARSY, COMMASY, SEMISY,
        COLONSY, ASSIGNSY, EQUSY,
        CONSTSY, VARSY, PROCEDURESY, ODDSY,
        WHILESY, DOSY, CALLSY, REPEATSY,
        UNTILSY, READSY, WRITESY,
        LESSSY, MORESY,
        LESSEQUALSY, MOREEQUALSY, NOTEQUALSY} symbol;

    QString ssymbol[36] = {"none","begin","end","if","then","else","identifer","int","double",
                             "plus","minus","star","divide","lpar","rpar","comma","semi",
                             "colon","assign","equal","const","var","procedure","odd",
                             "while","do","call","repeat","until","read","write","less","more",
                             "lessequal","moreequal","notequal"};
    //define funtions
    bool Getchar();
    void ClearToken(); //clear the token
    bool isSpace();
    bool isNewline();
    bool isTab();
    bool isLetter();
    bool isDigit();
    bool isColon(); //is :
    bool isComma(); //is ,
    bool isSemi(); //is ;
    bool isEqu();
    bool isPlus();
    bool isMinus();
    bool isDivi();
    bool isStar();
    bool isLpar();
    bool isRpar();
    bool isLess();
    bool isMore();
    bool CatToken(); // strcat for cur and token
    bool retract();
    Symbol reserver();
    void transNum();
    void error(QString);
    bool getsym();

public:
    QString analyze();

};

#endif // LEXANY_H
