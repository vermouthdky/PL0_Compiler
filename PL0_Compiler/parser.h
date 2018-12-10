#ifndef PARSER_H
#define PARSER_H

#include<lexany.h>
#include<pcodetable.h>
#include<symtable.h>

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
    int address = 0;
    const int addItem = 1;

private:
    void addErrorMessage(int kind, QString name);

public:
    Parser(QString input);
    Parser();
    bool GSAnalyse();
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
};

#endif // PARSER_H
