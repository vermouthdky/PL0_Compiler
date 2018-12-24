#include "lexany.h"

lexany::lexany(QString s)
{
    this->input = s;
    strptr = 0;
}

lexany::lexany(){}

QList<Token> lexany::analyze(){
    QList<Token> output;
    while(getsym()){
        if(symbol == Symbol::IDSY){
            //output.append(" "+tk+" "+ssymbol[symbol]+" "+tk);
            Token newitem = Token(symbol, linePtr, token);
            output.append(newitem);
        }else if(symbol == Symbol::DOUBLESY){
            //output.append(" "+tk+" "+ssymbol[symbol]+" "+QString::number(dnum));
            Token newitem = Token(symbol, linePtr, QString::number(dnum));
            output.append(newitem);
        }else if(symbol == Symbol::INTSY){
            //output.append(" "+tk+" "+ssymbol[symbol]+" "+QString::number(num));
            Token newitem = Token(symbol, linePtr, QString::number(num));
            output.append(newitem);
        }else if(symbol == Symbol::NONESY){
            //output.append(" "+tk+" "+ssymbol[symbol]+" "+"invalid-string!");
            Token newitem = Token(symbol, linePtr, "invalid-string");
            output.append(newitem);
        }else{
            //output.append(" "+tk+" "+ssymbol[symbol]+" "+"null");
            Token newitem = Token(symbol, linePtr, "null");
            output.append(newitem);
        }
    }
    //std::cout<<output.toStdString()<<std::endl;
    return output;
}

bool lexany::getsym(){
    ClearToken();
    do {
        if(!Getchar()) return false;
    }while(isSpace() || isNewline() || isTab()); //skip space, newline, and tab
    if(isLetter()){ //judge identifer or keyword
        CatToken();
        if(!Getchar()) return false;
        while(isLetter() || isDigit()){
            CatToken();
            if(!Getchar()) return false;
        }
        retract();
        Symbol resultValue = reserver();
        if(resultValue == Symbol::NONESY) symbol = Symbol::IDSY;
        else symbol = resultValue;
        return true;
        //else symbol=(Symbol)(resultValue-1);
    }
    else if(isDigit()){ //judge number
        while(isDigit()){
            CatToken();
            if(!Getchar()) return false;
        }
        if(cur == '.'){
            CatToken();
            if(!Getchar()) return false;
            while(isDigit()){
                CatToken();
                if(!Getchar()) return false;
            }
            retract();
            transNum();
            symbol = Symbol::DOUBLESY;
        }else if(isLetter()){
            symbol = Symbol::NONESY;
            retract();
            return true;
        }else{
            retract();
            transNum();
            symbol = Symbol::INTSY;
        }
    }
    else if(isColon()){ //judge := single : is invalid
        CatToken();
        if(!Getchar()) return false;
        if(isEqu()){
            symbol = Symbol::ASSIGNSY;
            CatToken();
            return true;
        }
        else{
            retract();
            symbol = Symbol::NONESY;
            return true;
        }
    }
    //below + - * ( ) , ; = < > <= >= <>
    else if(isPlus()){
        symbol = Symbol::PLUSSY;
        CatToken();
    }
    else if(isDot()){
        symbol = Symbol::DOTSY;
        CatToken();
    }
    else if(isMinus()){
        symbol = Symbol::MINUSSY;
        CatToken();
    }
    else if(isStar()){
        symbol = Symbol::STARSY;
        CatToken();
    }
    else if(isLpar()){
        symbol = Symbol::LPARSY;
        CatToken();
    }
    else if(isRpar()) {
        symbol = Symbol::RPARSY;
        CatToken();
    }
    else if(isComma()){
        symbol = Symbol::COMMASY;
        CatToken();
    }
    else if(isSemi()){
        symbol = Symbol::SEMISY;
        CatToken();
    }
    else if(isEqu()){
        symbol = Symbol::EQUSY;
        CatToken();
    }
    else if(isLess()){
        CatToken();
        Getchar();
        if(isEqu()){
            CatToken();
            symbol = Symbol::LESSEQUALSY;
        }
        else if(isMore()){
            CatToken();
            symbol = Symbol::NOTEQUALSY;
        }
        else{
            symbol = Symbol::LESSSY;
            retract();
        }
    }
    else if(isMore()){
        CatToken();
        if(!Getchar()) return false;
        if(isEqu()){
            CatToken();
            symbol = Symbol::MOREEQUALSY;
        }
        else{
            symbol = Symbol::MORESY;
            retract();
        }
    }
    else if(isDivi()){
        CatToken();
        if(!Getchar()) return false;
        if(isStar()){ //deal with the /*
            do{
                do { if(!Getchar()) return false; } while(!isStar());
                do {
                    if(!Getchar()) return false;
                    if(isDivi()) return true;
                }while(isStar());
            }while(!isStar());
        }
        else{ //当前/为除号
            retract();
            symbol = Symbol::DIVISY;
        }
    }
    else{
        CatToken();
        symbol = Symbol::NONESY;
    }
    //print();
    return true;
    //if() std::cout<<symbol<<std::endl;
    //return;
}

bool lexany::Getchar(){
    if(input[strptr] != '\0'){
        cur = input[strptr++];
        return true;
    }
    return false;
    //reader.get(cur);
}

void lexany::ClearToken() {
    token.clear();
}

bool lexany::isSpace() {
    return cur == ' ' ? true : false;
}

bool lexany::isNewline() {
    if(cur == '\n' || cur == '\r'){
        linePtr++;
        return true;
    }return false;
}

bool lexany::isTab() {
    return cur == '\t' ? true : false;
}

bool lexany::isLetter() {
    if((cur>='a'&&cur<='z') || (cur>='A'&&cur<='Z'))
        return true;
    return false;
}

bool lexany::isDigit() {
    if(cur<='9'&&cur>='0')
        return true;
    return false;
}

bool lexany::isColon() {
    return cur == ':' ? true : false;
}

bool lexany::isComma() {
    return cur == ',' ? true : false;
}

bool lexany::isSemi() {
    return cur == ';' ? true : false;
}

bool lexany::isEqu() {
    return cur == '=' ? true : false;
}

bool lexany::isPlus() {
    return cur == '+' ? true : false;
}

bool lexany::isMinus() {
    return cur == '-' ? true : false;
}

bool lexany::isDivi() {
    return cur == '/' ? true : false;
}

bool lexany::isStar() {
    return cur == '*' ? true : false;
}

bool lexany::isLpar() {
    return cur == '(' ? true : false;
}

bool lexany::isRpar() {
    return cur == ')' ? true : false;
}

bool lexany::isLess(){
    return cur == '<' ? true : false;
}

bool lexany::isMore(){
    return cur == '>' ? true : false;
}
bool lexany::isDot(){
    return cur == '.' ? true : false;
}
bool lexany::CatToken() {
    token.append(cur);
    return true;
}



bool lexany::retract() {
    if(strptr>0){
        strptr--;
        return true;
    }
    return false;
}

Symbol lexany::reserver() {
    if(!token.compare("begin")) return Symbol::BEGINSY;
    if(!token.compare("end")) return Symbol::ENDSY;
    if(!token.compare("if")) return Symbol::IFSY;
    if(!token.compare("then")) return Symbol::THENSY;
    if(!token.compare("else")) return Symbol::ELSESY;
    if(!token.compare("int")) return Symbol::INTSY;
    if(!token.compare("const")) return Symbol::CONSTSY;
    if(!token.compare("var")) return Symbol::VARSY;
    if(!token.compare("procedure")) return Symbol::PROCEDURESY;
    if(!token.compare("odd")) return Symbol::ODDSY;
    if(!token.compare("while")) return Symbol::WHILESY;
    if(!token.compare("do")) return Symbol::DOSY;
    if(!token.compare("call")) return Symbol::CALLSY;
    if(!token.compare("repeat")) return Symbol::REPEATSY;
    if(!token.compare("until")) return Symbol::UNTILSY;
    if(!token.compare("read")) return Symbol::READSY;
    if(!token.compare("write")) return Symbol::WRITESY;
    return Symbol::NONESY;
}

void lexany::transNum() {
    if(token.indexOf('.')!=-1){
        dnum = token.toDouble();
    }else {
        num = token.toInt();
    }
}

