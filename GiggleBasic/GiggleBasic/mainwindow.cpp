#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QRegularExpression"
#include "QDebug"
#include <QTextBrowser>
#include <QKeyEvent>
#include <stdexcept>
#include <QFileDialog>
#include <QDesktopServices>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1300,1100);//调整视图，更加美观
    ui->IdentifierEdit->setReadOnly(true);
    connect(ui->btnClearCode,&QPushButton::clicked,[=](){
        this->clear();
    });// 点击clear按钮时跳转到clear函数操作
    connect(ui->btnRunCode,&QPushButton::clicked,[=](){
        this->run();
    });// 点击run按钮时跳转到run函数操作
    connect(ui->btnLoadCode,&QPushButton::clicked,[=](){
        this->load();
    });// 点击load按钮时跳转到load函数操作
    connect(ui->cmdLineEdit,&QLineEdit::returnPressed,this,&MainWindow::codeLineEdit_return);//当捕捉到输入行有回车时，发出信号
    connect(ui->IdentifierEdit,&QLineEdit::returnPressed,[=](){
        try {
            QString tmpStr = ui->IdentifierEdit->text();
            if(!tmpStr.startsWith("? "))
                throw std::runtime_error("please just input a const num");
//            qDebug()<<tmpStr;
            bool ok;
            tmpStr = tmpStr.mid(2);
            tmpStr=tmpStr.trimmed();
            int numInt = tmpStr.toInt(&ok);
            myList.idList.valueIdentifier(identifierState->inputN->inputExp->name,numInt);
            ui->IdentifierEdit->clear();
            identifierState->inputN->inputCounter++;
            ui->IdentifierEdit->setReadOnly(true);
            this->condition = true;
        } catch (const std::runtime_error& e) {
            qDebug() << "Exception caught: " << e.what();
            ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(this->identifierState->codeNum));
            this->identifierState->runSucessfully = false;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAll()//该函数用于将三个展示栏内容清空
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    this->myList.list.clear();
    this->myList.idList.list.clear();
}

void MainWindow::codeLineEdit_return()//将输入的内容转换到代码框中并清除当前内容
{
    try {
        QString input = ui->cmdLineEdit->text();
        this->handleLine(input);
        ui->cmdLineEdit->clear();
        this->showCode();
    } catch (const MyException& e) {
        // 处理异常
//        qDebug() << "Caught exception: " << e.what();
        ui->errorShow->append(QString("Exception caught: %1").arg(e.what()));
    }

}

void MainWindow::handleLine(QString str)//该函数用于处理输入的命令
{
    str=str.trimmed();
//    qDebug()<<str;
    QRegularExpression regex("(\\d+)(.*)");
    QRegularExpressionMatch match = regex.match(str);
    bool startsWithNumberAndSpace = match.hasMatch();
    bool startWithNumber = (str[0] >= '0' && str[0] <= '9');
    //以上内容为判断开头是不是数字
//    qDebug()<<startsWithNumberAndSpace;
    if(startsWithNumberAndSpace && startWithNumber){
//        qDebug()<<"456";
        QRegularExpression re("(-?\\d+)(.*)");
        QRegularExpressionMatch match = re.match(str);

        QString number = match.captured(1); // 数字部分
        QString nonNumericPart = match.captured(2); // 非数字部分

        // 处理只有数字的情况
        if (number.isEmpty()) {
            number = str;
            nonNumericPart = "";
        }
        int num = number.toInt();
        this->numberStartHandleLine(num,nonNumericPart);
    }
    else{
        this->otherStartHandleLine(str);
    }

}

void MainWindow::numberStartHandleLine(int number, QString string)//该函数用于处理由数字开头的命令，参数分别为开头数字以及除数字之外的内容
{
    if(number <= 0 || number > 1000000){
        throw MyException("Code Number Error");
    }
    string = string.trimmed();
    myList.insert(number,string);
}

void MainWindow::otherStartHandleLine(QString string)//该函数用于处理不由数字开头的命令，参数为命令string
{
    bool isUsed = false;
//    qDebug()<<"other";
    if(string == "RUN")
    {
        this->run();
        isUsed = true;
    }
    if(string == "LOAD"){
        this->load();
        isUsed = true;
    }
    if(string == "CLEAR"){
        this->clear();
        isUsed = true;
    }

    if(string == "HELP"){
        this->help();
        isUsed = true;
    }

    if(string == "QUIT"){
        this->quit();
        isUsed = true;
    }

    if(string == "LIST"){
        isUsed = true;
    }

    if(string.startsWith("LET")){
        isUsed = true;
        this->singleLet(string);
    }
    if(string.startsWith("PRINT")){
        isUsed = true;
        this->singlePrint(string);
    }
    if(string.startsWith("INPUT")){
        isUsed = true;
        this->singleInput(string);
    }
//    qDebug()<<"wrong command1";
    qDebug()<<isUsed;
    if(isUsed == false){
//        qDebug()<<"wrong command";
        throw MyException("wrong command");
//        qDebug()<<"wrong command";
    }
}

void MainWindow::run()//进行函数运行
{
    try {
        if(myList.list.empty()){
            qDebug()<<"no code to run";
            throw std::runtime_error("no code to run!!!");
            return;
        }
        QLinkedList<oneCode>::iterator it;
        oneCode * code;
        for (it = myList.list.begin(); it != myList.list.end(); ++it) {
            code = &(*it);
            switch (code->state->com) {
            case REM:
                code->state->remN->remCounter = 0;
                break;
            case LET:
                code->state->letN->letCounter = 0;
                break;
            case PRINT:
                code->state->printN->printCounter = 0;
                break;
            case INPUT:
                code->state->inputN->inputCounter = 0;
                break;
            case GOTO:
                code->state->gotoN->gotoCounter = 0;
                break;
            case IF:
                code->state->ifN->rightCounter = 0;
                code->state->ifN->wrongCounter = 0;
                break;
            case END:
                code->state->endN->endCounter = 0;
                break;


            }

        }
       ui->treeDisplay->clear();
       ui->errorShow->clear();
       ui->textBrowser->clear();
       oneCode * currentCode = &this->myList.list.first();
       while(currentCode){
           this->runCode(currentCode);
       }
       this->showTree();
    } catch (const std::runtime_error& e) {
        qDebug() << "Exception caught: " << e.what();
        ui->errorShow->append(QString("Exception caught: %1").arg(e.what()));
    }



}

void MainWindow::load()//载入代码
{
    QFileDialog fileDialog;
    QString str;
    str = fileDialog.getOpenFileName(this,"请选定打开文件","C:/Users/23861/Desktop");
    file = new QFile(str);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw MyException("fail to open the file");
        }

        QTextStream in(file);
        QString content;
        while (!in.atEnd()) {
            QString line = in.readLine();
//            qDebug()<<line;
            this->handleLine(line);
        }
        file->close();
        this->showCode();

}

void MainWindow::clear()//清除工作
{
    myList.list.clear();
    myList.idList.list.clear();
    ui->treeDisplay->clear();
    ui->errorShow->clear();
    ui->textBrowser->clear();
    ui->CodeDisplay->clear();


}

void MainWindow::help()//帮助函数
{
    qDebug()<<"help open";
    QDesktopServices::openUrl(QUrl::fromLocalFile("../GiggleBasic/help.pdf"));
}

void MainWindow::quit()//退出函数
{
    this->close();
}

void MainWindow::showCode()//展示代码
{
    ui->CodeDisplay->clear();
    QLinkedList<oneCode>::iterator it;
    for (it = myList.list.begin(); it != myList.list.end(); ++it) {
        oneCode& code = *it;
        QString output = QString::number(code.codeNum) + " " + code.state->stateString;
        ui->CodeDisplay->append(output);
    }
//    QLinkedList<Identifier>::iterator it1;
//    for (it1 = myList.idList.list.begin(); it1 != myList.idList.list.end(); ++it1) {
//        QString output;
//        Identifier& iden = *it1;
//        if((it1->haveValue)){
//            output = "name: " + iden.name + " have value and num: "+QString::number(iden.value);
//        }else{
//            output = "name: " + iden.name + " have no value ";
//        }
//        ui->CodeDisplay->append(output);
//    }
}

void MainWindow::showTree()
{
    qDebug()<<"showTree";
    QLinkedList<oneCode>& list = this->myList.list;
    QLinkedList<oneCode>::iterator it = list.begin();
    QLinkedList<oneCode>::iterator endIt = list.end();
    oneCode *tmpCode;
    for (; it != endIt; ++it){
        tmpCode = &(*it);
        if(tmpCode->state->com == REM)
            this->remTree(tmpCode);
        if(tmpCode->state->com == LET)
            this->letTree(tmpCode);
        if(tmpCode->state->com == IF)
            this->ifTree(tmpCode);
        if(tmpCode->state->com == INPUT)
            this->inputTree(tmpCode);
        if(tmpCode->state->com == PRINT)
            this->printTree(tmpCode);
        if(tmpCode->state->com == GOTO)
            this->gotoTree(tmpCode);
        if(tmpCode->state->com == END)
            this->endTree(tmpCode);
    }


}

void MainWindow::runCode(oneCode *& code)//对当前代码进行运行
{
    oneCode * nextCode;
    QLinkedList<oneCode>& list = this->myList.list;
    // 查找 code 的迭代器
    QLinkedList<oneCode>::iterator it = list.begin();
    QLinkedList<oneCode>::iterator endIt = list.end();
    for (; it != endIt; ++it)
    {
        if (&(*it) == code)
        {
            ++it; // 移动迭代器到下一个元素
            if (it != endIt)
            {
                nextCode = &(*it);
            }else{
                nextCode = nullptr;
            }
            break;
        }
    }
    //以上内容初始化nextCode，如果code为最后一个元素，那么nextCode的初始化为nullptr，否则为下一条指令
    //下面开始添加一些处理statement的内容
    switch ((code)->state->com) {
    case REM:
        remRun(code->state);
        break;
    case LET:
        letRun(code->state);
        break;
    case PRINT:
        printRun(code->state);
        break;
    case IF:
        ifRun(code->state,nextCode);
        break;
    case INPUT:
        inputRun(code->state);
        break;
    case GOTO:
        gotoRun(code->state,nextCode);
        break;
    case END:
        endRun(code->state,nextCode);
        break;
    }
    

    
    
    
    
    code = nextCode;
}

void MainWindow::remTree(oneCode * code)//注释树构建
{
//    qDebug()<<"remTreeShowing";
//    qDebug()<<code->codeNum;
//    qDebug()<<QString(code->codeNum);
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " REM "+QString::number(code->state->remN->remCounter);
    ui->treeDisplay->append(firstLine);
    QString secondLine = "";
    for(int i = 0 ; i < 4 * 1 - 1;i++)
        secondLine += ' ';
    secondLine += code->state->remN->words;
    ui->treeDisplay->append(secondLine);
}
void MainWindow::letTree(oneCode * code)//let树构建
{
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " LET = "+QString::number(code->state->letN->letCounter);
    ui->treeDisplay->append(firstLine);
    QString secondLine = "";
    for(int i = 0;i < 4;i++){
        secondLine += ' ';
    }
    secondLine += code->state->letN->left->name;
    ui->treeDisplay->append(secondLine);
    QQueue<node*> queue;
    queue.enqueue(code->state->letN->right->btree->root);
    while (!queue.isEmpty()) {
        node * tmpNode = queue.front();
        queue.dequeue();
        if(tmpNode->left)
            queue.enqueue(tmpNode->left);
        if(tmpNode->right)
            queue.enqueue(tmpNode->right);
        QString output = "";
        for (int i = 0;i < 4 * tmpNode->height; i++) {
            output += ' ';
        }
        if(tmpNode->ntype == CONSTNUMBER)
            output += QString::number(tmpNode->constNum);
        if(tmpNode->ntype == IDENTIFIER)
            output += tmpNode->exp->name;
        if(tmpNode->ntype == SIGN){
            switch (tmpNode->stype) {
            case ADD:
                output += "+";
                break;
            case MINUS:
                output += "-";
                break;
            case MULTIPLY:
                output += "*";
                break;
            case DIVIDE:
                output += "/";
                break;
            case MOD:
                output += "MOD";
                break;
            case INDEX:
                output += "**";
                break;
            }
        }
        ui->treeDisplay->append(output);
    }

}
void MainWindow::printTree(oneCode * code)//print树构建
{
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " PRINT "+QString::number(code->state->printN->printCounter);
    ui->treeDisplay->append(firstLine);
    QQueue<node*> queue;
    queue.enqueue(code->state->printN->printExp->btree->root);
    while (!queue.isEmpty()) {
        node * tmpNode = queue.front();
        queue.dequeue();
        if(tmpNode->left)
            queue.enqueue(tmpNode->left);
        if(tmpNode->right)
            queue.enqueue(tmpNode->right);
        QString output = "";
        for (int i = 0;i < 4 * tmpNode->height; i++) {
            output += ' ';
        }
        if(tmpNode->ntype == CONSTNUMBER)
            output += QString::number(tmpNode->constNum);
        if(tmpNode->ntype == IDENTIFIER)
            output += tmpNode->exp->name;
        if(tmpNode->ntype == SIGN){
            switch (tmpNode->stype) {
            case ADD:
                output += "+";
                break;
            case MINUS:
                output += "-";
                break;
            case MULTIPLY:
                output += "*";
                break;
            case DIVIDE:
                output += "/";
                break;
            case MOD:
                output += "MOD";
                break;
            case INDEX:
                output += "**";
                break;
            }
        }
        ui->treeDisplay->append(output);
    }
}
void MainWindow::inputTree(oneCode * code)//input树构建
{
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " INPUT "+QString::number(code->state->inputN->inputCounter);
    ui->treeDisplay->append(firstLine);
    QString secondLine="";
    for(int i = 0;i < 4;i++){
        secondLine += ' ';
    }
    secondLine += code->state->inputN->inputExp->name;
    ui->treeDisplay->append(secondLine);
}
void MainWindow::gotoTree(oneCode * code)//goto树构建
{
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " GOTO "+QString::number(code->state->gotoN->gotoCounter);
    ui->treeDisplay->append(firstLine);
    QString secondLine="";
    for(int i = 0;i < 4;i++){
        secondLine += ' ';
    }
    secondLine += QString::number(code->state->gotoN->gotoNum);
    ui->treeDisplay->append(secondLine);
}
void MainWindow::ifTree(oneCode * code)//if树构建
{
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " IF THEN ";
    firstLine += QString::number(code->state->ifN->rightCounter);
    firstLine += ' ';
    firstLine += QString::number(code->state->ifN->wrongCounter);
    ui->treeDisplay->append(firstLine);
    QString base = "";
    for(int i = 0;i < 4;i++){
        base += ' ';
    }
    if(code->state->ifN->left->btree->root){
        this->showSingleNode(code->state->ifN->left->btree->root);
    }
    QString thirdLine = base;
    switch (code->state->ifN->op) {
    case greater:
        thirdLine += '>';
        break;
    case equal:
        thirdLine += '=';
        break;
    case less:
        thirdLine += '<';
        break;
    }
    ui->treeDisplay->append(thirdLine);
    if(code->state->ifN->right->btree->root){
        this->showSingleNode(code->state->ifN->right->btree->root);
    }
    QString fifthLine = base;
    fifthLine += QString::number(code->state->ifN->thenNum);
    ui->treeDisplay->append(fifthLine);
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }

    QQueue<node*> queue;
    if(code->state->ifN->left->btree->root->left)
        queue.enqueue(code->state->ifN->left->btree->root->left);
    if(code->state->ifN->left->btree->root->right)
        queue.enqueue(code->state->ifN->left->btree->root->right);
    if(code->state->ifN->right->btree->root->left)
        queue.enqueue(code->state->ifN->right->btree->root->left);
    if(code->state->ifN->right->btree->root->right)
        queue.enqueue(code->state->ifN->right->btree->root->right);

    while (!queue.isEmpty()) {
        node * tmpNode = queue.front();
        queue.dequeue();
        if(tmpNode->left)
            queue.enqueue(tmpNode->left);
        if(tmpNode->right)
            queue.enqueue(tmpNode->right);
        QString output = "";
        for (int i = 0;i < 4 * tmpNode->height; i++) {
            output += ' ';
        }
        if(tmpNode->ntype == CONSTNUMBER)
            output += QString::number(tmpNode->constNum);
        if(tmpNode->ntype == IDENTIFIER)
            output += tmpNode->exp->name;
        if(tmpNode->ntype == SIGN){
            switch (tmpNode->stype) {
            case ADD:
                output += "+";
                break;
            case MINUS:
                output += "-";
                break;
            case MULTIPLY:
                output += "*";
                break;
            case DIVIDE:
                output += "/";
                break;
            case MOD:
                output += "MOD";
                break;
            case INDEX:
                output += "**";
                break;
            }
        }
        ui->treeDisplay->append(output);
    }

}
void MainWindow::endTree(oneCode * code)//end树构建
{
    qDebug()<<"endTreeShowing";
//    qDebug()<<code->codeNum;
//    qDebug()<<QString(code->codeNum);
    if(!code->state->runSucessfully){
        this->errorTree(code);
        return;
    }
    QString firstLine = QString::number(code->codeNum) + " END " + QString::number(code->state->endN->endCounter);
    ui->treeDisplay->append(firstLine);

}

void MainWindow::errorTree(oneCode * code)//error树构建
{
    QString errorText = QString::number(code->codeNum) + " ERROR!!!";
    QString underline = QString(errorText.length(), '-'); // 使用连字符字符模拟下划线
    ui->treeDisplay->append(errorText);
    ui->treeDisplay->append(underline);
}


void MainWindow::showSingleNode(node *tmpNode)
{
    QString output = "";
    for (int i = 0;i < 4 * tmpNode->height; i++) {
        output += ' ';
    }
    if(tmpNode->ntype == CONSTNUMBER)
        output += QString::number(tmpNode->constNum);
    if(tmpNode->ntype == IDENTIFIER)
        output += tmpNode->exp->name;
    if(tmpNode->ntype == SIGN){
        switch (tmpNode->stype) {
        case ADD:
            output += "+";
            break;
        case MINUS:
            output += "-";
            break;
        case MULTIPLY:
            output += "*";
            break;
        case DIVIDE:
            output += "/";
            break;
        case MOD:
            output += "MOD";
            break;
        case INDEX:
            output += "**";
            break;
        }
    }
    ui->treeDisplay->append(output);
}

void MainWindow::remRun(Statement *state)//REM类型的state进行运行
{
    state->runSucessfully = true;
    qDebug()<<"rem running";
    state->remN->remCounter++;
    return;
}

void MainWindow::letRun(Statement * state)//LET类型的state进行运行
{
    state->runSucessfully = true;
    try {
        qDebug()<<"let running";
        int valueNum = state->letN->right->getValue();
        qDebug()<<"let value"<<valueNum;
        this->myList.idList.valueIdentifier(state->letN->left->name,valueNum);
        state->letN->letCounter++;
    } catch (const std::runtime_error& e) {
        qDebug() << "Exception caught: " << e.what();
        ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(state->codeNum));
        state->runSucessfully = false;
    }

}
void MainWindow::printRun(Statement * state)//PRINT类型的state进行运行
{
    state->runSucessfully = true;
    try {
        qDebug()<<"print running";
        QString output = QString::number(state->printN->printExp->getValue());
        ui->textBrowser->append(output);
        state->printN->printCounter++;
    } catch (const std::runtime_error& e) {
        qDebug() << "Exception caught: " << e.what();
        ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(state->codeNum));
        state->runSucessfully = false;
    }

}
void MainWindow::inputRun(Statement* state)
{
        state->runSucessfully = true;
        qDebug() << "input running";
        ui->IdentifierEdit->setText("? ");
        ui->IdentifierEdit->setCursorPosition(2);  // 设置光标位置在文本结尾
        this->identifierState = state;
        ui->IdentifierEdit->setReadOnly(false);
        QEventLoop loop;
        while (!condition) {
            loop.processEvents();
        }
        condition = false;
}
void MainWindow::gotoRun(Statement * state,oneCode *&code)//GOTO类型的state进行运行
{
    state->runSucessfully = true;
    qDebug()<<"goto running";
    int num = state->gotoN->gotoNum;
    QLinkedList<oneCode>::iterator it;
    for (it = myList.list.begin(); it != myList.list.end(); ++it) {
        if((*it).codeNum == num){
            code = &(*it);
            state->gotoN->gotoCounter++;
            return;
        }
    }
    try {
            throw std::runtime_error("WRONG GOTO NUM");
        } catch (const std::runtime_error& e) {
            qDebug() << "Exception caught: " << e.what();
            ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(state->codeNum));
        }
    state->runSucessfully = false;
    qDebug()<<"still running";
}
void MainWindow::ifRun(Statement * state,oneCode * &nextcode)//IF类型的state进行运行
{
    state->runSucessfully = true;
    try {
        qDebug()<<"if running";
        int leftValue = state->ifN->left->getValue();
        int rightValue  = state->ifN->right->getValue();
        bool result = false;
        if(leftValue > rightValue && state->ifN->op == greater)
            result = true;
        if(leftValue == rightValue && state->ifN->op == equal)
            result = true;
        if(leftValue < rightValue && state->ifN->op == less)
            result = true;
        if(!result)
            state->ifN->wrongCounter++;
        if(result){

            qDebug()<<"then running";
            int num = state->ifN->thenNum;
            QLinkedList<oneCode>::iterator it;
            for (it = myList.list.begin(); it != myList.list.end(); ++it) {
                if((*it).codeNum == num){
//                    qDebug()<<"num"<<num;
                    nextcode = &(*it);
                    state->ifN->rightCounter++;
//                    qDebug()<<"n"<<nextcode->codeNum;
                    return;
                }
            }
            try {
                    throw std::runtime_error("WRONG THEN NUM");
                } catch (const std::runtime_error& e) {
//                    qDebug() << "Exception caught: " << e.what();
                    ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(state->codeNum));
                }
            state->runSucessfully = false;
            qDebug()<<"still running";
        }

    } catch (const std::runtime_error& e) {
        qDebug() << "Exception caught: " << e.what();
        ui->errorShow->append(QString("Exception caught: %1 at line %2").arg(e.what()).arg(state->codeNum));
        state->runSucessfully = false;
    }





}
void MainWindow::endRun(Statement * state,oneCode *&code)//END类型的state进行运行
{
    state->runSucessfully = true;
    qDebug()<<"end running";
    code = nullptr;
    state->endN->endCounter++;
}

void MainWindow::singleLet(QString str)
{
    letState = new Statement(str,&this->myList.idList);
    letState->codeNum = -1;
    letRun(letState);
}

void MainWindow::singleInput(QString str)
{
    inputState = new Statement(str,&this->myList.idList);
    inputState->codeNum = -2;
    inputRun(inputState);
}

void MainWindow::singlePrint(QString str)
{
    qDebug()<<"printing";
    printState = new Statement(str,&this->myList.idList);
    printState->codeNum = -3;
    printRun(printState);
}
