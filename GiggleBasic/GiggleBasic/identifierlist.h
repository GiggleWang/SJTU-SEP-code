#pragma once
#ifndef IDENTIFIERLIST_H
#define IDENTIFIERLIST_H
#include "QObject"
#include "QLinkedList"
#include "QDebug"
#include "myexception.h"
struct Identifier{
    QString name;
    bool haveValue = false;
    int value;
};

class IdentifierList
{
public:
    QLinkedList<Identifier> list;
    IdentifierList();
    bool findIdentifier(QString str);
    void insertIdentifier(QString str);
    void valueIdentifier(QString str,int val);
    bool identifierHaveValue(QString str);
    int takeValue(QString str);
};

#endif // IDENTIFIERLIST_H
