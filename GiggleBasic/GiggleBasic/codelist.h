#pragma once
#ifndef CODELIST_H
#define CODELIST_H
#include "statement.h"
#include "QLinkedList"
#include "QDebug"
#include "myexception.h"
#include "identifierlist.h"

struct oneCode{
    int codeNum;
    Statement* state = nullptr;
};

class CodeList
{
private:

public:
    QLinkedList<oneCode> list;
    IdentifierList idList;
    void insert(int codeNum,QString str);
    void remove(int codeNum);
    bool find(int codeNum);
    CodeList();
};

#endif // CODELIST_H
