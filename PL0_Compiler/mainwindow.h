#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include "lexany.h"
#include <parser.h>
#include <interpreter.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_file_triggered();
    void on_compileButton_clicked();

    void on_interpretButton_clicked();

    QString enumtovalue(PCodeOperator);

private:
    Ui::MainWindow *ui;
    void addTreeitem(QString, QString, QString);
    PCodeTable temtable;

};

#endif // MAINWINDOW_H
