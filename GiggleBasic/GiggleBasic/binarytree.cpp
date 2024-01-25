#include "binarytree.h"
#include <QString>

BinaryTree::BinaryTree(QString str,IdentifierList * l)
{
    //对传入参数进行保存
    this->list = l;
    root = newBinaryTree(str,1);
}
void BinaryTree::clear()
{
    //清除树
    clearBinaryTree(root);
}
void BinaryTree::clearBinaryTree(node * &n)
{
    //如果节点为空，直接返回
    if(n == nullptr)
        return;
    //清除左右节点
    clearBinaryTree(n->left);
    clearBinaryTree(n->right);
    delete n;
    n = nullptr;
}
node* BinaryTree::newBinaryTree(QString str,int initialHeight){
    str = str.trimmed();
    if(str == ""){
        QString error = "wrong compoundedExp!";
        throw MyException(error);
    }
    node * retNode;
    //检测最后一次出现+ or - or * or / or MOD or **且不在（）中的情况

    //+
    int lastPlusIndex = -1;
    bool flag = true;
    int counter = 0;
    for(int i = 0;i < str.length();i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == '+' && counter < 1)
            lastPlusIndex = i;
    }
    //-
    int lastMinusIndex = -1;
    counter = 0;
    for(int i = 0;i < str.length();i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == '-' && counter < 1 && i != 0)
            lastMinusIndex = i;
    }
    //*
    int lastMultiplyIndex = -1;
    counter = 0;
    for(int i = 0;i < str.length();i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == '*' && counter < 1 && (i == 0 || str[i-1] != '*')&&(i == str.length() - 1 || str[i+1] != '*'))
            lastMultiplyIndex = i;
    }
    // /
    int lastDivideIndex = -1;
    counter = 0;
    for(int i = 0;i < str.length();i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == '/' && counter < 1)
            lastDivideIndex = i;
    }
    // **
    int lastIndexIndex = -1;
    counter = 0;
    for(int i = 0;i < str.length()-1;i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == '*' && str[i+1] == '*' && counter < 1)
            lastIndexIndex = i;
    }
    //MOD
    // **
    int lastModIndex = -1;
    counter = 0;
    for(int i = 0;i < str.length() - 3;i++){
        if(str[i] == '(')
            counter++;
        if(str[i] == ')')
            counter--;
        if(str[i] == 'M' && str[i+1] == 'O' && str[i+2] == 'D' && counter < 1)
            lastModIndex = i;
    }

    //首先检测是否有+或者-，如果有，则将string切割为最后一次出现+或-的两部分
    if(lastPlusIndex > -1 || lastMinusIndex > -1){
        int splitIndex = qMax(lastPlusIndex, lastMinusIndex); // 找到最后一个"+"或"-"的索引
        QString part1 = str.left(splitIndex); // 切割点左侧的部分
        QString part2 = str.right(str.length() - splitIndex - 1); // 切割点右侧的部分
        SignType tmpSign;
        if(lastPlusIndex > lastMinusIndex){
            tmpSign = ADD;
        }else{
            tmpSign = MINUS;
        }
        qDebug() << "have + or -";
        qDebug() << "Part 1:" << part1;
        qDebug() << "Part 2:" << part2;
        qDebug();
        retNode = new node(tmpSign,initialHeight,newBinaryTree(part1,initialHeight + 1),newBinaryTree(part2,initialHeight + 1));
        return  retNode;
    }

    //然后检测是否含有*或者/或者MOD，如果有，则将string切割为最后一次出现*或/或MOD的两部分
    if((lastMultiplyIndex >= 0) || (lastDivideIndex >= 0) || (lastModIndex >= 0)){
        int lastIndex = qMax(lastMultiplyIndex,qMax(lastDivideIndex,lastModIndex));
        SignType s;
        if(lastIndex == lastModIndex)
            s = MOD;
        if(lastIndex == lastDivideIndex)
            s = DIVIDE;
        if(lastIndex == lastMultiplyIndex)
            s = MULTIPLY;
        QString part1 = str.left(lastIndex);
        QString part2;
        if (lastIndex == lastModIndex)
            part2 = str.mid(lastIndex + 3);
        else
            part2 = str.mid(lastIndex + 1);

//        qDebug()<<lastMultiplyIndex;
//        qDebug()<<lastDivideIndex;
//        qDebug()<<lastMultiplyIndex;
        qDebug() << "have * or / or MOD";
        qDebug() << "Part 1:" << part1;
        qDebug() << "Part 2:" << part2;
        qDebug();
        retNode = new node(s,initialHeight,newBinaryTree(part1,initialHeight + 1),newBinaryTree(part2,initialHeight + 1));
        return  retNode;
    }


    //然后检测是否含有**，如果有，则将string切割为最后一次出现**的两部分

    if(lastIndexIndex > -1){
        int splitIndex = lastIndexIndex;
        QString part1 = str.left(splitIndex); // 切割点左侧的部分
        QString part2 = str.mid(splitIndex + 2); // 切割点右侧的部分
        SignType tmpSign = INDEX;
        qDebug() << "have **";
        qDebug() << "Part 1:" << part1;
        qDebug() << "Part 2:" << part2;
        qDebug();
        retNode = new node(tmpSign,initialHeight,newBinaryTree(part1,initialHeight + 1),newBinaryTree(part2,initialHeight + 1));
        return  retNode;
    }


    //开头和末尾是(和)并且中间没有先出现)，那么剥去括号
    if(str[0] == '(' && str[str.length()-1] == ')'){
        QString tmpStr = str;
        tmpStr.remove(0, 1);
        tmpStr.remove(tmpStr.length()-1,1);
        //检验是否先出现右括号
        int leftCounter = 0;
        int rightCounter = 0;
        bool counterflag = true;
        for(int i = 0;i < tmpStr.length();i++){
            if(rightCounter > leftCounter)
                counterflag = false;
            if(str[i] == '(')
                leftCounter++;
            if(str[i] == ')')
                rightCounter++;
        }
        if(counterflag){
            qDebug()<<tmpStr;
            return newBinaryTree(tmpStr,initialHeight);
        }
    }

    //如果所有的元素都是数字，那么记为const
    flag = true;
    for(int i = 0;i < str.length();i++){
        if(str[i] < '0' || str[i] > '9')
            flag = false;
    }
    if(flag){
        int retInt = str.toInt();
        retNode = new node(retInt,initialHeight,nullptr,nullptr);
        return retNode;
    }

    //如果除了第一个元素所有的元素都是数字，那么记为const
    flag = true;
    for(int i = 1;i < str.length();i++){
        if(str[i] < '0' || str[i] > '9')
            flag = false;
    }

    if(flag && str[0] == '-'){
        QString tmpStr = str;
        tmpStr.remove(0,1);
        int retInt = -tmpStr.toInt();
//        qDebug()<<str;
//        qDebug()<<tmpStr;
//        qDebug()<<retInt;
        retNode = new node(retInt,initialHeight,nullptr,nullptr);
        return retNode;
    }

    //如果均不满足上面的情况，那么只能是一个identifierExp
    IdentifierExp * retExp = new IdentifierExp(str,this->list);
    qDebug()<<"retExp->name  ";
    qDebug()<<retExp->name;
    retNode = new node(retExp,initialHeight,nullptr,nullptr);
    return retNode;
    //((A+B-C)MODd*E/F)
}
