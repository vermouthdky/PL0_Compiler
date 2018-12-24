#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->codeTextedit->setFont(QFont(tr("Consolas")));
    ui->inputTextedit->setFont(QFont(tr("Consolas"), 24));
    ui->outputBrowser->setFont(QFont(tr("Consolas"), 24));
    ui->errorBrowser->setFont(QFont(tr("Consolas"), 20));
    ui->errorBrowser->setTextColor(Qt::red);
//    QFileSystemModel *treeModel = new QFileSystemModel(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionopen_file_triggered(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/",
                                                    tr("PL code File (*.txt *.pl0)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){

    }
    QTextStream out(&file);
    QString code = out.readAll();
    ui->codeTextedit->setText(code);
//    lexany analyzer(code);
//    QList<Token> output = analyzer.analyze();
}



void MainWindow::on_compileButton_clicked()
{
    Parser parser(ui->codeTextedit->toPlainText());
    bool success = parser.GSAnalyse();
    if(success){
        ui->errorBrowser->clear();
        temtable = parser.getPCodeTable();
        QList<PerPCode> pcodetable = temtable.getPCodeTable();
        QTableWidget *ptableptr = ui->pcodeTablewiget;
        ptableptr->setColumnCount(3);
        ptableptr->setRowCount(pcodetable.size());

        QStringList header;
        header<<"OP"<<"X"<<"Y";
        ptableptr->setHorizontalHeaderLabels(header);
        QStringList verticalHeader;
        for(int i=0;i<pcodetable.size();i++) verticalHeader<<QString::number(i);
        ptableptr->setVerticalHeaderLabels(verticalHeader);
        for(int i=0;i<pcodetable.size();i++){
            ptableptr->setItem(i, 0, new QTableWidgetItem(enumtovalue(pcodetable[i].getOP())));
            ptableptr->setItem(i, 1, new QTableWidgetItem(QString::number((pcodetable[i].getX()))));
            ptableptr->setItem(i, 2, new QTableWidgetItem(QString::number(pcodetable[i].getY())));
        }
    }else{
        ui->pcodeTablewiget->clear();
        QStringList errorList = parser.getErrorMessage();
        ui->errorBrowser->setText(errorList.join("\n"));
    }
}

QString MainWindow::enumtovalue(PCodeOperator x){
    if(x == PCodeOperator::CAL) return "CAL";
    else if(x == PCodeOperator::INT) return "INT";
    else if(x == PCodeOperator::JMP) return "JMP";
    else if(x == PCodeOperator::JPC) return "JPC";
    else if(x == PCodeOperator::LIT) return "LIT";
    else if(x == PCodeOperator::LOD) return "LOD";
    else if(x == PCodeOperator::OPR) return "OPR";
    else if(x == PCodeOperator::STO) return "STO";
    else if(x == PCodeOperator::RED) return "RED";
    else if(x == PCodeOperator::WRT) return "WRT";
    else return "WRO";
}


void MainWindow::on_interpretButton_clicked()
{
    QString input = ui->inputTextedit->toPlainText();
    QList<int> intInput;
    lexany lexer(input);
    QList<Token> inputList = lexer.analyze();
    for(int i=0;i<inputList.size();i++){
        if(inputList[i].getSy() == Symbol::INTSY){
            intInput.append(inputList[i].getValue().toInt());
        }
    }
    interpreter interpreter(intInput);
    interpreter.setPcodeTable(temtable);
    QString output = "";
    try{
        output = interpreter.run();
    }catch(...){
        output = "Unexpected failure.\n";
    }
    ui->outputBrowser->setText(output);
}
