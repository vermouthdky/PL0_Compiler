#include "lexany.h"

lexany::lexany(QString s)
{
    this->input = s;
    strptr = 0;
}

lexany::lexany(){}

QString lexany::analyze(){
    QString output = "";
    while(getsym()){
        QString tk = token;
        if(symbol == IDSY){
            //std::cout<<ssymbol[symbol]<<" "<<token<<std::endl;
            output.append(" "+tk+" "+ssymbol[symbol]+" "+tk);
        }else if(symbol == DOUBLESY){
            //std::cout<<ssymbol[symbol]<<" "<<dnum<<std::endl;
            output.append(" "+tk+" "+ssymbol[symbol]+" "+QString::number(dnum));
        }else if(symbol == INTSY){
            //std::cout<<ssymbol[symbol]<<" "<<num<<std::endl;
            output.append(" "+tk+" "+ssymbol[symbol]+" "+QString::number(num));
        }else if(symbol == NONESY){
            output.append(" "+tk+" "+ssymbol[symbol]+" "+"invalid-string!");
        }else{
            //std::cout<<ssymbol[symbol]<<std::endl;
            output.append(" "+tk+" "+ssymbol[symbol]+" "+"null");
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
        if(resultValue == NONESY) symbol = IDSY;
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
            symbol = DOUBLESY;
        }else if(isLetter()){
            symbol = NONESY;
            retract();
            return true;
        }else{
            retract();
            transNum();
            symbol = INTSY;
        }
    }
    else if(isColon()){ //judge := single : is invalid
        CatToken();
        if(!Getchar()) return false;
        if(isEqu()){
            symbol = ASSIGNSY;
            CatToken();
            return true;
        }
        else{
            retract();
            symbol = NONESY;
            return true;
        }
    }
    //below + - * ( ) , ; = < > <= >= <>
    else if(isPlus()){
        symbol = PLUSSY;
        CatToken();
    }
    else if(isMinus()){
        symbol = MINUSSY;
        CatToken();
    }
    else if(isStar()){
        symbol = STARSY;
        CatToken();
    }
    else if(isLpar()){
        symbol = LPARSY;
        CatToken();
    }
    else if(isRpar()) {
        symbol = RPARSY;
        CatToken();
    }
    else if(isComma()){
        symbol = COMMASY;
        CatToken();
    }
    else if(isSemi()){
        symbol = SEMISY;
        CatToken();
    }
    else if(isEqu()){
        symbol = EQUSY;
        CatToken();
    }
    else if(isLess()){
        CatToken();
        Getchar();
        if(isEqu()){
            CatToken();
            symbol = LESSEQUALSY;
        }
        else if(isMore()){
            CatToken();
            symbol = NOTEQUALSY;
        }
        else{
            symbol = LESSSY;
            retract();
        }
    }
    else if(isMore()){
        CatToken();
        if(!Getchar()) return false;
        if(isEqu()){
            CatToken();
            symbol = MOREEQUALSY;
        }
        else{
            symbol = MORESY;
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
            symbol = DIVISY;
        }
    }
    else{
        CatToken();
        symbol = NONESY;
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
    return cur == '\n' || cur == '\r' ? true : false;
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

lexany::Symbol lexany::reserver() {
    if(!token.compare("begin")) return BEGINSY;
    if(!token.compare("end")) return ENDSY;
    if(!token.compare("if")) return IFSY;
    if(!token.compare("then")) return THENSY;
    if(!token.compare("else")) return ELSESY;
    if(!token.compare("int")) return INTSY;
    if(!token.compare("const")) return CONSTSY;
    if(!token.compare("var")) return VARSY;
    if(!token.compare("procedure")) return PROCEDURESY;
    if(!token.compare("odd")) return ODDSY;
    if(!token.compare("while")) return WHILESY;
    if(!token.compare("do")) return DOSY;
    if(!token.compare("call")) return CALLSY;
    if(!token.compare("repeat")) return REPEATSY;
    if(!token.compare("until")) return UNTILSY;
    if(!token.compare("read")) return READSY;
    if(!token.compare("write")) return WRITESY;
    return NONESY;
}

void lexany::transNum() {
    if(token.indexOf('.')!=-1){
        dnum = token.toDouble();
    }else {
        num = token.toInt();
    }
}

