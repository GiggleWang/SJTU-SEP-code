#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "identifierexp.h"
#include <QtCore>
#include <QRegularExpression>
#include <QString>
#include "identifierlist.h"
Q_ENUMS(NodeType)
enum NodeType{
    CONSTNUMBER,IDENTIFIER,SIGN
};
enum SignType{
    ADD,MINUS,MULTIPLY,DIVIDE,MOD,INDEX
};

struct node{
    NodeType ntype;
    int constNum;
    IdentifierExp * exp;
    SignType stype;
    int height;
    node* left;
    node* right;
    node() : left(NULL), right(NULL) { }
    node(int cNum,int h,node *l = nullptr,node* r = nullptr){
        constNum = cNum;
        left = l;
        right = r;
        ntype = CONSTNUMBER;
        height = h;
    }
    node(IdentifierExp * e,int h,node *l = nullptr,node* r = nullptr){
        exp = e;
        left = l;
        right = r;
        ntype = IDENTIFIER;
        height = h;
    }
    node(SignType s,int h,node *l = nullptr,node* r = nullptr){
        stype = s;
        left = l;
        right = r;
        ntype = SIGN;
        height = h;
    }
    ~node() {}
};
class BinaryTree
{
public:
    node* root;
    IdentifierList * list;
    BinaryTree(QString str,IdentifierList *l);
    void clear();
    void clearBinaryTree(node * &n);
    node* newBinaryTree(QString str,int initialHeight);
};

#endif // BINARYTREE_H
