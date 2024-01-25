#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H
#include "QObject"
#include "identifierexp.h"
#include "compoundexp.h"
#include "QDebug"
#include "myexception.h"
#include "identifierlist.h"
Q_ENUMS(Command)
Q_ENUMS(Op)
enum Command{
    REM,LET,PRINT,INPUT,GOTO,IF,END
};
enum Op{
    greater,equal,less
};
class Statement
{
public:


    struct remNode{
        const QString commandString = "REM";
        QString words;
        int remCounter = 0;
    };
    struct letNode{
        const QString commandString = "LET";
        IdentifierExp * left;
        CompoundExp * right;
        int letCounter = 0;
    };
    struct printNode{
        const QString commandString = "PRINT";
        CompoundExp * printExp;
        int printCounter = 0;
    };
    struct inputNode{
        const QString commandString = "INPUT";
        IdentifierExp * inputExp;
        int inputCounter = 0;
    };
    struct gotoNode{
        const QString commandString = "GOTO";
        int gotoNum;
        int gotoCounter = 0;
    };
    struct ifNode{
        const QString commandString = "IF THEN";
        CompoundExp * left;
        CompoundExp * right;
        int thenNum;
        Op op;
        int rightCounter = 0;
        int wrongCounter = 0;
    };
    struct endNode{
        const QString commandString = "END";
        int endCounter = 0;
    };



    Statement(QString string , IdentifierList* list);
    int codeNum;
//    int counter = 0;
    bool runSucessfully = true;
    IdentifierList * list;
    QString stateString;
    Command com;
    remNode* remN;
    letNode* letN;
    printNode* printN;
    inputNode* inputN;
    gotoNode* gotoN;
    ifNode* ifN;
    endNode* endN;
    void remStart(QString str);
    void letStart(QString lstring,QString rstring);
    void printStart(QString str);
    void inputStart(QString str);
    void gotoStart(QString str);
    void ifStart(QString lstring,QString opString,QString rstring,QString afterThen);
    void endStart();
};

#endif // STATEMENT_H
