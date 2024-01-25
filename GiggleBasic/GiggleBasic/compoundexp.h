#pragma once
#ifndef COMPOUNDEXP_H
#define COMPOUNDEXP_H
#include "QObject"
#include "myexception.h"
#include "identifierlist.h"
#include "binarytree.h"
#include <QtMath>
class CompoundExp
{
public:
    IdentifierList * list;
    BinaryTree * btree;
    QString cExpStr;
    CompoundExp(QString str,IdentifierList * l);
    int getValue();
    int getNodeValue(node * node);
};

#endif // COMPOUNDEXP_H
