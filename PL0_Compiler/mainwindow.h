#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include "lexany.h"
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
private:
    Ui::MainWindow *ui;
    void addTreeitem(QString, QString, QString);

};

#endif // MAINWINDOW_H
