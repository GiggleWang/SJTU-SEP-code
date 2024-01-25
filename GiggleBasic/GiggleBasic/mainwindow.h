#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "codelist.h"
#include "myexception.h"
#include "identifierlist.h"
#include "identifierexp.h"
#include "statement.h"
#include <QQueue>
#include <QApplication>
#include <QTextBrowser>
#include <QEventLoop>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Statement * identifierState = nullptr;
    CodeList myList;
    bool condition = false;
    Statement * letState;
    Statement * printState;
    Statement * inputState;
    //用于存储文件
    QFile* file;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void handleLine(QString str);
    void numberStartHandleLine(int number,QString string);
    void otherStartHandleLine(QString string);
    void run();
    void load();
    void clear();
    void help();
    void quit();
    void showCode();
    void showTree();
    void runCode(oneCode *& code);
    void remTree(oneCode * code);
    void letTree(oneCode * code);
    void printTree(oneCode * code);
    void inputTree(oneCode * code);
    void gotoTree(oneCode * code);
    void ifTree(oneCode * code);
    void endTree(oneCode * code);
    void errorTree(oneCode * code);
    void showSingleNode(node * n);
    void remRun(Statement * state);
    void letRun(Statement * state);
    void printRun(Statement * state);
    void inputRun(Statement * state);
    void gotoRun(Statement * state,oneCode *&code);
    void ifRun(Statement * state,oneCode * &code);
    void endRun(Statement * state,oneCode *&code);
    void singleLet(QString str);
    void singlePrint(QString str);
    void singleInput(QString str);
private slots:
    void clearAll();
    void codeLineEdit_return();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
