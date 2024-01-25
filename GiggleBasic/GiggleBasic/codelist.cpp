#include "codelist.h"


bool compareCode(const oneCode& code1, const oneCode& code2) {
    //该函数用于比较两个code的序号值
    return code1.codeNum < code2.codeNum;
}
CodeList::CodeList()
{

}

void CodeList::insert(int num,QString str)//该函数作用为将代码序号为num，代码内容为str的语句插入list
{
//    qDebug() << num;
//    qDebug() << str;
    if(str == ""){
//        qDebug()<<"removed";
        remove(num);
        return;
    }
    oneCode code;
    code.codeNum = num;
    code.state = new Statement(str,&this->idList);
    code.state->codeNum = num;
    if(this->find(num)){
        remove(num);
    }
    // 找到插入的位置
    QLinkedList<oneCode>::iterator it = std::lower_bound(list.begin(), list.end(), code, compareCode);
    // 在合适的位置插入对象
    list.insert(it, code);
}

void CodeList::remove(int num)//该函数用于删除序列号为num的code
{
    if (this->find(num)) {
        QLinkedList<oneCode>::iterator it = list.begin();
        while (it != list.end()) {
            if (it->codeNum == num) {
                it = list.erase(it);
            } else {
                ++it;
            }
        }
    }

}

bool CodeList::find(int num)//该函数用来寻找list表中是否有以num为代码编号的oneCode
{
    QLinkedList<oneCode>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); ++it) {
        if((*it).codeNum == num)
            return true;
    }
    return false;
}
