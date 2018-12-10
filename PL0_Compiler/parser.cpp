#include "parser.h"

Parser::Parser(QString input)
{
    lexer = lexany(input);
    tokentable = lexer.analyze();
}

bool Parser::GSAnalyse(){
    program();
    return (!isError);
}

void Parser::program(){
    block();
    if(tokentable[tokenPtr].getSy()==Symbol::PROCEDURESY){
        tokenPtr++;
        if(tokentable[tokenPtr].getSy() != Symbol::EOF){
            errorMessage();
        }
    }
}
