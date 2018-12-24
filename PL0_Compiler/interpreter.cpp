#include "interpreter.h"

interpreter::interpreter(){
    pc = 0;
    base = 0;
    top = 0;
    stack[0] = stack[1] = stack[2] = 0;
}

interpreter::interpreter(QList<int> input){
    this->input = input;
    pc = 0;
    base = 0;
    top = 0;
    stack[0] = stack[1] = stack[2] = 0;
}

void interpreter::setPcodeTable(PCodeTable pcodetable){
    this->pcode = pcodetable.getPCodeTable();
}

QStringList interpreter::getOutput(){
    return output;
}

QString interpreter::run(){
    QString output = "";
    int inputPtr = 0;
    pc = 0;
    base = 0;
    top = 0;
    stack[0] = stack[1] = stack[2] = 0;
    do{
        PerPCode curPCode = pcode.at(pc);
        pc++;
        if(curPCode.getOP() == PCodeOperator::LIT){
            stack[top++] = curPCode.getY();
        }else if(curPCode.getOP() == PCodeOperator::OPR){
            switch(curPCode.getY()){
            case 0:
                top = base;
                pc = stack[base+2];
                base = stack[base];
                break;
            case 1:
                stack[top-1] = -stack[top-1];
                break;
            case 2:
                stack[top-2] += stack[top-1];
                top--;
                break;
            case 3:
                stack[top-2] -=stack[top-1];
                top--;
                break;
            case 4:
                stack[top-2] *=stack[top-1];
                top--;
                break;
            case 5:
                stack[top-2] /=stack[top-1];
                top--;
                break;
            case 6:
                stack[top-1] = stack[top-1]%2;
                break;
            case 7:
                stack[top-1]==stack[top-2]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;
            case 8:
                stack[top-1]!=stack[top-2]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;
            case 9:
                stack[top-2]<stack[top-1]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;
            case 10:
                stack[top-2]>=stack[top-1]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;
            case 11:
                stack[top-2]>stack[top-1]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;
            case 12:
                stack[top-2]<=stack[top-1]?stack[top-2]=1:stack[top-2]=0;
                top--;
                break;

            }
        }else if(curPCode.getOP() == PCodeOperator::LOD){
            stack[top] = stack[curPCode.getY() + getBase(base, curPCode.getX())];
            top++;
        }else if(curPCode.getOP() == PCodeOperator::STO){
            stack[curPCode.getY() + getBase(base, curPCode.getX())] = stack[top-1];
            top--;
        }else if(curPCode.getOP() == PCodeOperator::CAL){
            stack[top] = base;
            stack[top+1] = getBase(base, curPCode.getX());
            stack[top+2] = pc;
            base = top;
            pc = curPCode.getY();
        }else if(curPCode.getOP() == PCodeOperator::INT){
            top = top + curPCode.getY();
        }else if(curPCode.getOP() == PCodeOperator::JMP){
            pc = curPCode.getY();
        }else if(curPCode.getOP() == PCodeOperator::JPC){
            if(stack[top-1] == 0){
                pc = curPCode.getY();
            }
        }else if(curPCode.getOP() == PCodeOperator::RED){
            if(inputPtr < input.size()){
                stack[top++] = input[inputPtr++];
            }else{
                output.append("expect input\n");
                return output;
            }
        }else if(curPCode.getOP() == PCodeOperator::WRT){
            if(curPCode.getY() == 0){
                output.append(QString::number(stack[top-1])+" ");
            }else{
                output.append("\n");
            }
        }
    }while(pc != 0);
    return output;
}

int interpreter::getBase(int BP, int lev){
    while (lev > 0) {
        BP = stack[BP+1];
        lev--;
    }
    return BP;
}
