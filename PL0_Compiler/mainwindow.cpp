#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QFileSystemModel *treeModel = new QFileSystemModel(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionopen_file_triggered(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/",
                                                    tr("PL code File (*.txt *.pl0 *.c)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){

    }
    QTextStream out(&file);
    QString code = out.readAll();
    lexany analyzer(code);
    QString output = analyzer.analyze();
    QStringList qlist = output.split(" ");

    for(int i = 1; i < qlist.size(); i+=3){
        addTreeitem(qlist.at(i), qlist.at(i+1), qlist.at(i+2));
    }

}

void MainWindow::addTreeitem(QString token, QString symbol, QString value){

    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->testtree);

    treeItem->setText(0, token);
    treeItem->setText(1, symbol);
    treeItem->setText(2, value);
}
