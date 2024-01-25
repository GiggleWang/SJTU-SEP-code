#include "identifierlist.h"

IdentifierList::IdentifierList()
{

}
bool IdentifierList::findIdentifier(QString str)
{
    //寻找当前的变量是否已经存在
    QLinkedList<Identifier>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); ++it) {
        if((*it).name == str)
            return true;
    }
    return false;
}
void IdentifierList::insertIdentifier(QString str)
{
    //插入变量
    if(this->findIdentifier(str)){
        return;
    }//如果已经可以找到，就直接return
    Identifier id;
    id.name = str;
    list.append(id);
}
void IdentifierList::valueIdentifier(QString str,int val)
{
    //变量赋值
    if(!this->findIdentifier(str)){
        this->insertIdentifier(str);
    }
    QLinkedList<Identifier>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if ((*it).name == str){
            (*it).value = val;
            (*it).haveValue = true;
        }
    }
}

bool IdentifierList::identifierHaveValue(QString str)
{
    //查询当前变量是不是已经被赋值
    QLinkedList<Identifier>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); ++it) {
        if((*it).name == str && (*it).haveValue ==true)
            return true;
    }
    return false;
}
int IdentifierList::takeValue(QString str)
{
    //查询变量赋值
    QLinkedList<Identifier>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); ++it) {
        if((*it).name == str && (*it).haveValue ==true)
            return (*it).value;
    }
}
