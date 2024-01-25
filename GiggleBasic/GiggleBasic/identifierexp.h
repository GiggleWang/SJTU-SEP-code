#pragma once
#ifndef IDENTIFIEREXP_H
#define IDENTIFIEREXP_H
#include "QObject"
#include "myexception.h"
#include <QDebug>
#include <QRegExp>
#include "identifierlist.h"
class IdentifierExp
{
public:
    QString name;
    int counter = 0;
    IdentifierList * list;
    IdentifierExp(QString str,IdentifierList * list);
    bool isValidVariableName(const QString& variableName);
};

#endif // IDENTIFIEREXP_H
