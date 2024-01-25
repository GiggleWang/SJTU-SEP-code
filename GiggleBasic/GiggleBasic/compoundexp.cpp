#include "compoundexp.h"

CompoundExp::CompoundExp(QString str,IdentifierList * l)
{
    //对传入参数进行保存
    this->list = l;
    this->cExpStr = str;
    this->btree = new BinaryTree(str,this->list);
}
int CompoundExp::getValue()
{
    //对表达式进行求值
    return getNodeValue(this->btree->root);
}
int CompoundExp::getNodeValue(node * node)
{
    //对node的值进行解析
    //如果为空，那么报错
    if(node == nullptr)
        throw std::runtime_error("have null identifier");
    //如果是立即数
    if(node->ntype == CONSTNUMBER)
        return node->constNum;
    //如果是变量
    if(node->ntype == IDENTIFIER){
        if(!this->list->findIdentifier(node->exp->name)){
            throw std::runtime_error("identifier not defined yet");
        }
        if(!this->list->identifierHaveValue(node->exp->name)){
            throw std::runtime_error("identifier not valued yet");
        }
        return this->list->takeValue(node->exp->name);
    }
    //如果是符号
    if(node->ntype == SIGN){
        switch (node->stype) {
        case ADD:
            return getNodeValue(node->left)+getNodeValue(node->right);
            break;
        case MINUS:
            return getNodeValue(node->left)-getNodeValue(node->right);
            break;
        case MULTIPLY:
            return getNodeValue(node->left)*getNodeValue(node->right);
            break;
        case DIVIDE:
            return getNodeValue(node->left)/getNodeValue(node->right);
            break;
        case MOD:{
            if(getNodeValue(node->right) == 0)
                throw std::runtime_error("number after mod cannot be 0");
            int a = getNodeValue(node->left);
            int b = getNodeValue(node->right);
            int ret = a - (a / b ) * b;
            if(b < 0){
                ret = ret + b;
            }
            return ret;
            break;
        }
        case INDEX:
            return qPow(getNodeValue(node->left),getNodeValue(node->right));
            break;
        }
    }
}
