#ifndef LEXANY_H
#define LEXANY_H
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <cmath>

class lexany
{
public:
    lexany(std::string);
    lexany();

private:
    int strptr;
    std::string input;
    char cur;
    char token[100];
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

    std::string ssymbol[36] = {"none","begin","end","if","then","else","identifer","int","double",
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
    void error(std::string);
    bool getsym();
    std::string binary(int);
    std::string binary(double);

public:
    std::string analyze();

};

#endif // LEXANY_H
