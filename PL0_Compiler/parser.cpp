#include "parser.h"

Parser::Parser(QString input)
{
    lexer = lexany(input);
    tokentable = lexer.analyze();
}

bool Parser::GSAnalyse(){
    try{
        program();
    }catch(ReadErrorException e){
        addErrorMessage(29, "");
    }
    return (!isError);
}

void Parser::program(){
    block();
    if(tokentable[tokenPtr].getSy() == Symbol::DOTSY){}
//    else{
//        addErrorMessage(30, "");
//    }
//    if(tokentable[tokenPtr].getSy()==Symbol::PROCEDURESY){
//        nextPtr();
//        if(tokentable[tokenPtr].getSy() != Symbol::NONESY){
//            addErrorMessage(-1, "");
//        }
//    }else{
//        addErrorMessage(-1,"");
//    }
}

void Parser::block(){
    int preAddress = address;
    //initiat current layer variable
    int start = symtable.getPtr();
    int pos = 0;
    address = 3;
    if(start > 0){
        pos = symtable.getLevelProc(level-1);
    }
    int temPcodePtr = pcodetable.getPCodePtr();
    pcodetable.addPerPCode(PCodeOperator::JMP, 0, 0);


    if(tokentable[tokenPtr].getSy() == Symbol::CONSTSY){
        constDeclare();
    }
    if(tokentable[tokenPtr].getSy() == Symbol::VARSY){
        varDeclare();
    }
    if(tokentable[tokenPtr].getSy() == Symbol::PROCEDURESY){
        procDeclare();
    }


    pcodetable.setY(temPcodePtr,pcodetable.getPCodePtr());
    pcodetable.addPerPCode(PCodeOperator::INT, 0, newAddress());
    if(start != 0){
        symtable.getSymbolTable()[pos].setValue(pcodetable.getPCodePtr()-1-symtable.getSymbolTable()[pos].getSize());
    }
    statement();
    pcodetable.addPerPCode(PCodeOperator::OPR, 0, 0);
    address = preAddress;

}

void Parser::constDeclare(){

    if(tokentable[tokenPtr].getSy() == Symbol::CONSTSY){
        nextPtr();
        constDefine();
        while(tokentable[tokenPtr].getSy() == Symbol::COMMASY){
            nextPtr();
            constDefine();
        }
        if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
            nextPtr();
        }else{
            addErrorMessage(5,"");
        }
    }else{ //lack const
        addErrorMessage(-1,"");
    }
}

void Parser::constDefine(){
    QString IDname;
    QString Numvalue;
    if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
        IDname = tokentable[tokenPtr].getValue();
        nextPtr();
        if(tokentable[tokenPtr].getSy() == Symbol::EQUSY){
            nextPtr();
            if(tokentable[tokenPtr].getSy() == Symbol::INTSY){
                Numvalue = tokentable[tokenPtr].getValue();
                if(symtable.thisLayerExisted(IDname, level)){
                    addErrorMessage(26, "");
                }else if(Numvalue.length()>=10){
                    addErrorMessage(25, Numvalue);
                }else{
                    symtable.enterConst(IDname, level, Numvalue.toInt(), address);
                }
                nextPtr();
            }
        }else{
            addErrorMessage(3,IDname);
        }
    }else{
        addErrorMessage(4,"");
    }
}

void Parser::varDeclare(){
    QString IDname;
    if(tokentable[tokenPtr].getSy() == Symbol::VARSY){
        nextPtr();
        if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
            IDname = tokentable[tokenPtr].getValue();
            if(symtable.thisLayerExisted(IDname, level)){
                addErrorMessage(26,IDname);
            }
            else{
                symtable.enterVar(IDname, level, newAddress());
            }
            nextPtr();
            while(tokentable[tokenPtr].getSy() == Symbol::COMMASY){
                nextPtr();
                if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
                    IDname = tokentable[tokenPtr].getValue();
                    if(symtable.thisLayerExisted(IDname, level)){
                        addErrorMessage(26,IDname);
                    }
                    else{
                        symtable.enterVar(IDname, level, newAddress());
                    }
                    nextPtr();
                }else{
                    addErrorMessage(-1,"");
                }
            }

            if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
                nextPtr();
            }else{
                addErrorMessage(10,"");
            }
        }else{
            addErrorMessage(4,"");
        }

    }else{
        addErrorMessage(-1,"");
    }
}

void Parser::procDeclare(){
    if(tokentable[tokenPtr].getSy() == Symbol::PROCEDURESY){
        nextPtr();
        int pos = 0;
        if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
            QString name = tokentable[tokenPtr].getValue();
            if(symtable.thisLayerExisted(name, level)){
                addErrorMessage(26, name);
            }
            pos = symtable.getPtr();
            symtable.enterProcess(name, level, address);
            level++;
            nextPtr();
            if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
                nextPtr();
            }else{
                addErrorMessage(10, "");
            }
            block();
            symtable.clearLayer(level);
            while(tokentable[tokenPtr].getSy() == Symbol::SEMISY || tokentable[tokenPtr].getSy() == Symbol::PROCEDURESY){
                if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
                    nextPtr();
                }else{
                    addErrorMessage(10, "");
                }
                level--;
                procDeclare();
            }
        }else{
            addErrorMessage(-1, "");
        }
    }

//    if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
//        nextPtr();
//    }else{
//        addErrorMessage(10, "");
//    }
}

void Parser::complexStatement(){
    if(tokentable[tokenPtr].getSy() == Symbol::BEGINSY){
        nextPtr();
        statement();
        while(tokentable[tokenPtr].getSy() == Symbol::SEMISY ||
              isStatementHeader()){
            if(tokentable[tokenPtr].getSy() == Symbol::SEMISY){
                nextPtr();
            }else{ // ; expected
                addErrorMessage(10, "");
            }
            statement();
        }
        if(tokentable[tokenPtr].getSy() == Symbol::ENDSY){
            nextPtr();
        }else{//end expected
            addErrorMessage(19, "");
        }
    }else{//begin expected
        addErrorMessage(2, "");
    }
}

void Parser::statement(){

    if(tokentable[tokenPtr].getSy() == Symbol::IFSY){
        nextPtr();
        condition();
        if(tokentable[tokenPtr].getSy() == Symbol::THENSY){
            int tempos = pcodetable.getPCodePtr();
            pcodetable.addPerPCode(PCodeOperator::JPC, 0, 0);
            nextPtr();
            statement();
            int tempos2 = pcodetable.getPCodePtr();
            pcodetable.addPerPCode(PCodeOperator::JMP, 0, 0);
            pcodetable.setY(tempos,pcodetable.getPCodePtr());
            //in case no else below, assign JMP current ptr
            pcodetable.setY(tempos2,pcodetable.getPCodePtr());
            if(tokentable[tokenPtr].getSy() == Symbol::ELSESY){
                nextPtr();
                statement();
                pcodetable.setY(tempos2,pcodetable.getPCodePtr());
            }
        }else{
            addErrorMessage(16,"");
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::WHILESY){
        //<while> ::= while <condition> do <statement>
        int tempos = pcodetable.getPCodePtr();
        nextPtr();
        condition();
        if(tokentable[tokenPtr].getSy() == Symbol::DOSY){
            nextPtr();
            int tempos2 = pcodetable.getPCodePtr();
            pcodetable.addPerPCode(PCodeOperator::JPC, 0, 0);
            statement();
            pcodetable.addPerPCode(PCodeOperator::JMP, 0, tempos);
            pcodetable.setY(tempos2,pcodetable.getPCodePtr());
        }else{
            addErrorMessage(18, "");
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::CALLSY){
        //<callprocess> ::= call <identifer>
        nextPtr();
        PerSym temsym;
        if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
            QString name = tokentable[tokenPtr].getValue();
            if(symtable.preLayerExisted(name, level)){
                temsym = symtable.getSymbol(name);
                if(temsym.getType() == symtable.getProc()){
                    pcodetable.addPerPCode(PCodeOperator::CAL, level-temsym.getLevel(), temsym.getValue());
                    nextPtr();
                }else{
                    addErrorMessage(14,"");
                }
            }else{
                addErrorMessage(11,"");
                return;
            }
        }else{
            addErrorMessage(14,"");
            return;
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::READSY){
        //<read> ::= read'('<identifer>{,<identifer>}')'
        nextPtr();
        if(tokentable[tokenPtr].getSy() == Symbol::LPARSY){
            nextPtr();
            if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
                QString name = tokentable[tokenPtr].getValue();
                if(symtable.preLayerExisted(name, level)){
                    PerSym temsym = symtable.getSymbol(name);
                    if(temsym.getType() == symtable.getVar()){
                        pcodetable.addPerPCode(PCodeOperator::RED, 0, 0);
                        pcodetable.addPerPCode(PCodeOperator::STO, level-temsym.getLevel(), temsym.getAddress());
                    }else{
                        addErrorMessage(12,name);
                        return;
                    }
                }else{
                    addErrorMessage(11, name);
                    return;
                }
            }else{
                addErrorMessage(-1,"");
            }
            nextPtr();
            while(tokentable[tokenPtr].getSy() == Symbol::COMMASY){
                nextPtr();
                if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
                    QString name = tokentable[tokenPtr].getValue();
                    if(symtable.preLayerExisted(name, level)){
                        PerSym temsym = symtable.getSymbol(name);
                        if(temsym.getType() == symtable.getVar()){
                            //OPR 0 16 read
                            //pcodetable.addPerPCode(PCodeOperator::OPR, 0, 16);
                            pcodetable.addPerPCode(PCodeOperator::RED, 0, 0);
                            pcodetable.addPerPCode(PCodeOperator::STO, level-temsym.getLevel(), temsym.getAddress());
                        }else{
                            addErrorMessage(12,name);
                            return;
                        }
                    }else{
                        addErrorMessage(11, name);
                        return;
                    }
                }else{
                    addErrorMessage(-1,"");
                    return;
                }
                nextPtr();
            }
            if(tokentable[tokenPtr].getSy() == Symbol::RPARSY){
                nextPtr();
            }else{
                addErrorMessage(22,"");
                return;
            }

        }else{
            addErrorMessage(-1,"");
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::WRITESY){
        nextPtr();
        if(tokentable[tokenPtr].getSy() == Symbol::LPARSY){
            nextPtr();
            expression();
            // OPR 0 14 write
            pcodetable.addPerPCode(PCodeOperator::WRT, 0, 0);
            while(tokentable[tokenPtr].getSy() == Symbol::COMMASY){
                nextPtr();
                expression();
                pcodetable.addPerPCode(PCodeOperator::WRT, 0, 0);
            }
            pcodetable.addPerPCode(PCodeOperator::WRT, 0, 1);
            if(tokentable[tokenPtr].getSy() == Symbol::RPARSY){
                nextPtr();
            }else{//lack )
                addErrorMessage(22, "");
                return;
            }
        }else{
            addErrorMessage(-1, "");
            return;
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::BEGINSY){
        //complex statement
        complexStatement();
    }else if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
        QString name = tokentable[tokenPtr].getValue();
        nextPtr();
        if(tokentable[tokenPtr].getSy() == Symbol::ASSIGNSY || tokentable[tokenPtr].getSy() == Symbol::EQUSY){
            if(tokentable[tokenPtr].getSy() == Symbol::EQUSY){
                addErrorMessage(1, "");
            }
            nextPtr();
            expression();
            if(symtable.preLayerExisted(name, level)){
                PerSym temsym = symtable.getSymbol(name);
                if(temsym.getType() == symtable.getVar()){
                    pcodetable.addPerPCode(PCodeOperator::STO, level-temsym.getLevel(), temsym.getAddress());
                }else{
                    addErrorMessage(12, name);
                }
            }else{
                addErrorMessage(11, name);
                return;
            }
        }else{
            addErrorMessage(-1,"");
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::REPEATSY){
        nextPtr();
        int tempos = pcodetable.getPCodePtr();
        statement();
        while(tokentable[tokenPtr].getSy() == Symbol::SEMISY || isStatementHeader()){
            if(isStatementHeader()){
                addErrorMessage(10, "");
            }else{
                nextPtr();
            }
            if(tokentable[tokenPtr].getSy() == Symbol::UNTILSY){
                addErrorMessage(7, "");
                break;
            }
            statement();
        }
        if(tokentable[tokenPtr].getSy() == Symbol::UNTILSY){
            nextPtr();
            condition();
            pcodetable.addPerPCode(PCodeOperator::JPC, 0, tempos);
        }else{
            addErrorMessage(28, "");
        }
    }else{
        addErrorMessage(7, "");
    }
}

void Parser::condition(){
    if(tokentable[tokenPtr].getSy() == Symbol::ODDSY){
        pcodetable.addPerPCode(PCodeOperator::OPR, 0, 6);
        nextPtr();
        expression();
    }else{
        expression();
        Symbol tem = tokentable[tokenPtr].getSy();
        nextPtr();
        expression();
        if(tem == Symbol::EQUSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 7);
        }else if(tem == Symbol::NOTEQUALSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 8);
        }else if(tem == Symbol::LESSSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 9);
        }else if(tem == Symbol::MOREEQUALSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 10);
        }else if(tem == Symbol::MORESY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 11);
        }else if(tem == Symbol::LESSEQUALSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 12);
        }else{
            addErrorMessage(20, tokentable[tokenPtr].getValue());
        }
    }
}

void Parser::expression(){
    Symbol tem = tokentable[tokenPtr].getSy();
    if(tem == Symbol::PLUSSY || tem == Symbol::MINUSSY){
        nextPtr();
    }
    term();
    if(tem == Symbol::MINUSSY){
        pcodetable.addPerPCode(PCodeOperator::OPR, 0, 1);
    }
    while(tokentable[tokenPtr].getSy() == Symbol::PLUSSY ||
          tokentable[tokenPtr].getSy() == Symbol::MINUSSY){
        tem = tokentable[tokenPtr].getSy();
        nextPtr();
        term();
        if(tem == Symbol::PLUSSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 2);
        }else if(tem == Symbol::MINUSSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 3);
        }
    }
}

void Parser::term(){
    factor();
    while(tokentable[tokenPtr].getSy() == Symbol::STARSY ||
          tokentable[tokenPtr].getSy() == Symbol::DIVISY){
        Symbol tem = tokentable[tokenPtr].getSy();
        nextPtr();
        factor();
        if(tem == Symbol::STARSY){
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 4);
        }else{
            pcodetable.addPerPCode(PCodeOperator::OPR, 0, 5);
        }
    }
}

void Parser::factor(){
    if(tokentable[tokenPtr].getSy() == Symbol::INTSY){
        pcodetable.addPerPCode(PCodeOperator::LIT, 0, tokentable[tokenPtr].getValue().toInt());
        nextPtr();
    }else if(tokentable[tokenPtr].getSy() == Symbol::LPARSY){
        nextPtr();
        expression();
        if(tokentable[tokenPtr].getSy() == Symbol::RPARSY){
            nextPtr();
        }else{
            addErrorMessage(22, "");
            return;
        }
    }else if(tokentable[tokenPtr].getSy() == Symbol::IDSY){
        QString name = tokentable[tokenPtr].getValue();
        if(symtable.preLayerExisted(name, level)){
            PerSym temsym = symtable.getSymbol(name);
            if(temsym.getType() == symtable.getVar()){
                pcodetable.addPerPCode(PCodeOperator::LOD, level-temsym.getLevel(), temsym.getAddress());
            }else if(temsym.getType() == symtable.getCon()){
                pcodetable.addPerPCode(PCodeOperator::LIT, 0, temsym.getValue());
            }else{
                addErrorMessage(21, name);
            }
            nextPtr();
        }else{
            addErrorMessage(11, name);
        }
    }else {
        addErrorMessage(31, "");
    }
}
/*
 * use the error message of COP 2016
1. Use = instead of :=.
2. = must be followed by a number.
3. Identifier must be followed by =.
4. const, var, procedure must be followed by identifier.
5. Semicolon or comma missing.
6. Incorrect symbol after procedure declaration.
7. Statement expected.
8. Incorrect symbol after statement part in block.
9. Period expected.
10. Semicolon between statements missing.
11. Undeclared identifier.
12. Assignment to constant or procedure is not allowed.
13. Assignment operator expected.
14. call must be followed by an identifier.
15. Call of a constant or variable is meaningless.
16. then expected.
17. Semicolon or } expected.
18. do expected.
19. Incorrect symbol following statement.
20. Relational operator expected.
21. Expression must not contain a procedure identifier.
22. Right parenthesis missing.
23. The preceding factor cannot begin with this symbol.
24. An expression cannot begin with this symbol.
25. This number is too large.
26. Duplicate declaration for identifer.
27. begin expected.
28. until expected.
29. incomplete code
*/
void Parser::addErrorMessage(int kind, QString name){
    isError = true;
    QString erroritem;
    switch (kind) {
    case 1:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Use = instead of :=.";
        break;
    case 2:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"= must be followed by a number.";
        break;
    case 3:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+name+" "+"Identifier must be followed by =.";
        break;
    case 4:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"const, var, procedure must be followed by identifier.";
        break;
    case 5:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Semicolon or comma missing.";
        break;
    case 6:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Incorrect symbol after procedure declaration.";
        break;
    case 7:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"duplicated semicolon ; ";
        break;
    case 8:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Incorrect symbol after statement part in block.";
        break;
    case 9:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Period expected.";
        break;
    case 10:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Semicolon between statements missing.";
        break;
    case 11:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+name+" "+"Undeclared identifier.";
        break;
    case 12:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Assignment to constant or procedure is not allowed.";
        break;
    case 13:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Assignment operator expected.";
        break;
    case 14:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"call must be followed by an identifier.";
        break;
    case 15:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"call of a constant or variable is meaningless.";
        break;
    case 16:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"then expected.";
        break;
    case 17:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Semicolon or } expected.";
        break;
    case 18:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"do expected.";
        break;
    case 19:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Incorrect symbol following statement.";
        break;
    case 20:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Relational operator expected.";
        break;
    case 21:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Expression must not contain a procedure identifier.";
        break;
    case 22:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Right parenthesis missing.";
        break;
    case 23:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"The preceding factor cannot begin with this symbol.";
        break;
    case 24:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"An expression cannot begin with this symbol.";
        break;
    case 25:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+name+"This number is too large.";
        break;
    case 26:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"Duplicate declaration for const identifer";
        break;
    case 27:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"begin expected.";
        break;
    case 28:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"until expected.";
        break;
    case 29:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"incomplete code.";
        break;
    case 30:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"dot expected";
        break;
    case 31:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"incomplete expression.";
        break;
    default:
        erroritem = "line "+QString::number(tokentable[tokenPtr].getLine())+": "+"unexpected error.";
        break;
    }
    errorMessage.append(erroritem);
}

bool Parser::isStatementHeader(){
    Symbol tem = tokentable[tokenPtr].getSy();
    if(tem == Symbol::IFSY || tem == Symbol::WHILESY ||
       tem == Symbol::CALLSY || tem == Symbol::READSY ||
       tem == Symbol::WRITESY || tem == Symbol::BEGINSY){
        return true;
    }return false;
}



