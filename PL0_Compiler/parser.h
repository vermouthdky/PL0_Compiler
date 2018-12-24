#ifndef PARSER_H
#define PARSER_H

#include<lexany.h>
#include<pcodetable.h>
#include<symtable.h>
#include<exception>
#include<readerrorexception.h>

class Parser
{
private:
    lexany lexer;
    QList<Token> tokentable;
    PCodeTable pcodetable;
    SymTable symtable;
    QStringList errorMessage;

    bool isError = false;
    int tokenPtr = 0;

    int level = 0;
    int address = -1;
    const int addItem = 1;

private:
    void program();
    void block();
    void constDeclare();
    void constDefine();
    void uInt();
    void indentifer();
    void varDeclare();
    void procDeclare();
    void procHeader();
    void statement();
    void assignStatement();
    void expression();
    void term();
    void factor();
    void plusOperator();
    void multiOperator();
    void condition();
    void relationalOperator();
    void conditionStatement();
    void whenWhile();
    void callProcess();
    void complexStatement();
    void readStatement();
    void writeStatement();
    void letter();
    void number();
    void addErrorMessage(int kind, QString token);
    void nextPtr(){
        if(tokenPtr<tokentable.size()-1){
            tokenPtr++;
        }else {
            throw ReadErrorException();
        }
    }

    int newAddress(){
        return address++;
    }
    bool isStatementHeader();

public:
    Parser(QString input);
    Parser();
    bool GSAnalyse();
    QStringList getErrorMessage(){
        return errorMessage;
    }
    QList<Token> getTokenTable(){
        return tokentable;
    }
    SymTable getSymTable(){
        return symtable;
    }
    PCodeTable getPCodeTable(){
        return pcodetable;
    }

};

#endif // PARSER_H
