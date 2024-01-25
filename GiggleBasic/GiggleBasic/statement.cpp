#include "statement.h"

Statement::Statement(QString str , IdentifierList * l)
{
    this->list = l;
    bool flag = false;
    str = str.trimmed();
    stateString = str;
    if(str.startsWith("REM")){
        com = REM;
        QString tmp = str.mid(3);
        this->remStart(tmp);
        flag = true;
    }
    if(str.startsWith("LET")){
        com = LET;
        QString tmp = str.mid(3);
        tmp = tmp.trimmed();
        if(!tmp.contains("=")){
            throw MyException("Loss of the op equal");
            return;
        }
        QStringList parts = tmp.split("=");
        QString beforeEqual = parts[0];  // 等号前面的部分
        QString afterEqual = parts[1];   // 等号后面的部分
        beforeEqual = beforeEqual.trimmed();
        afterEqual = afterEqual.trimmed();
        this->letStart(beforeEqual,afterEqual);
        flag = true;
    }
    if(str.startsWith("PRINT")){
        com = PRINT;
        QString tmp = str.mid(5);
        tmp = tmp.trimmed();
        this->printStart(tmp);
        flag = true;
    }
    if(str.startsWith("INPUT")){
        com = INPUT;
        QString tmp = str.mid(5);
        tmp = tmp.trimmed();
        this->inputStart(tmp);
        flag = true;
    }
    if(str.startsWith("GOTO")){
        com = GOTO;
        QString tmp = str.mid(4);
        tmp = tmp.trimmed();
        this->gotoStart(tmp);
        flag = true;
    }
    if(str.startsWith("IF")){
        com  = IF;
        QString opString;
        QString tmp = str.mid(2);
        tmp = tmp.trimmed();
        if(!tmp.contains("THEN")){
            throw MyException("Loss of the word THEN");
            return;
        }
        QStringList parts = tmp.split("THEN");
        QString beforeThen = parts[0];  // THEN前面的部分
        QString afterThen = parts[1];   // THEN后面的部分
        bool containsGreater = str.contains(">");
        bool containsEqual = str.contains("=");
        bool containsLess = str.contains("<");
        if ((containsGreater && !containsEqual && !containsLess) ||
            (!containsGreater && containsEqual && !containsLess) ||
            (!containsGreater && !containsEqual && containsLess)) {
            // 只有一个为true，其他为false的情况
            if(containsGreater){
                opString = ">";
            }
            if(containsEqual){
                opString = "=";
            }
            if(containsLess){
                opString = "<";
            }
        } else {
            // 不满足只有一个为true，其他为false的情况
            throw MyException("IFTHEN have op problem");
        }
        QStringList parts1 = beforeThen.split(opString);
        QString beforeOp = parts1[0];  // THEN前面的部分
        QString afterOp = parts1[1];   // THEN后面的部分
        beforeOp = beforeOp.trimmed();
        opString = opString.trimmed();
        afterOp = afterOp.trimmed();
        afterThen = afterThen.trimmed();
        this->ifStart(beforeOp,opString,afterOp,afterThen);
        flag = true;
    }
    if(str.startsWith("END")){
        com = END;
        this->endStart();
        flag = true;
    }
    if(!flag){
        throw MyException("No statement found");
    }
}
void Statement::remStart(QString str)//构建remNode
{
    this->remN = new remNode;
    remN->words = str;
}
void Statement::letStart(QString lstring,QString rstring)//构建letNode
{
    this->letN = new letNode;
    letN->left = new IdentifierExp(lstring , this->list);
    letN->right = new CompoundExp(rstring , this->list);
}
void Statement::printStart(QString str)//构建printNode
{
    this->printN = new printNode;
    printN->printExp = new CompoundExp(str , this->list);
}
void Statement::inputStart(QString str)//构建inputNode
{
    this->inputN = new inputNode;
    inputN->inputExp = new IdentifierExp(str , this->list);
}
void Statement::gotoStart(QString str)//构建gotoNode
{
    this->gotoN = new gotoNode;
    str = str.trimmed();
    bool isNumber = false;
    int intValue = str.toInt(&isNumber);
    if(!isNumber){
        throw MyException("Target line should be a constant");
        return;
    }
    gotoN->gotoNum = intValue;
}
void Statement::ifStart(QString lstring,QString opString,QString rstring,QString afterThen)//构建ifNode
{
    this->ifN = new ifNode;
    ifN->left = new CompoundExp(lstring , this->list);
    ifN->right = new CompoundExp(rstring , this->list);
    if(opString == ">")
        ifN->op = greater;
    if(opString == "=")
        ifN->op = equal;
    if(opString == "<")
        ifN->op = less;
    bool isNumber = false;
    int intValue = afterThen.toInt(&isNumber);
    if(!isNumber){
        throw MyException("After THEN should be a constant");
        return;
    }
    ifN->thenNum = intValue;
}
void Statement::endStart()//构建endNode
{
    this->endN = new endNode;
}
