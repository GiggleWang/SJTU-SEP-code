#include "identifierexp.h"

IdentifierExp::IdentifierExp(QString str,IdentifierList * l)
{
    //对传入参数进行保存
    str = str.trimmed();
    if(!this->isValidVariableName(str)){
        QString output = str + "Variable name is not valid";
        throw MyException(output);
    }
    this->name = str;
    this->list = l;
}

bool IdentifierExp::isValidVariableName(const QString& variableName) {
    // 检查长度
    if (variableName.length() < 1 || variableName.length() > 255) {
        return false;
    }

    // 检查格式
    QRegExp regex("[a-zA-Z_][a-zA-Z0-9_]*");
    if (!regex.exactMatch(variableName)) {
        return false;
    }

    return true;
}
