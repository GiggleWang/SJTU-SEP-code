#include "singleplayscene.h"
#include <QPainter>
#include <QtGlobal>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileDialog>

SinglePlayScene::SinglePlayScene(QWidget *parent) : QWidget(parent)//该函数为单人模式下的构造函数
{
    //设置抓取键盘，从而在操控小人时可以使用上下左右键
    this->grabKeyboard();
    setFocusPolicy(Qt::NoFocus);
    //设定固定大小
    this->setFixedSize(2200,1300);
    this->setWindowTitle("单人模式");
    //设置相关按钮初始化
    this->setButtons();
    //如果点击“退出”按钮，回到上一界面
    connect(getBackBtn,&MyButton::released,[=](){
        emit this->singlePlayClose();
    });
    this->setBackground();
    this->setPlayBtn();
    //初始化player
    player = new Player(":/resource3/player.jpg");
    player->setParent(this);
    this->updatePlayerLocation();
    this->solveSignals();
}

void SinglePlayScene::setButtons()//该函数用于初始化界面中的按钮
{
    //设置返回按钮
    getBackBtn = new MyButton(":/resource/GetBackButton.png");
    getBackBtn->setParent(this);
    getBackBtn->setFixedSize(400,150);
    getBackBtn->move(this->width() - getBackBtn->width() - 200,1100);
    //设置游戏暂停按钮
    gameStopBtn = new QPushButton();
    gameStopBtn->setParent(this);
    gameStopBtn->setFixedSize(300,150);
    QPixmap pix;
    pix.load(":/resource3/gameStopBtn.png");
    gameStopBtn->setStyleSheet("QPushButton{border:0px;}");
    gameStopBtn->setIcon(pix);
    gameStopBtn->setIconSize(QSize(pix.width(),pix.height()));
    gameStopBtn->move(50,1100);
    //设置游戏继续按钮
    gameContinueBtn = new QPushButton();
    gameContinueBtn->setParent(this);
    gameContinueBtn->setFixedSize(300,150);
    pix.load(":/resource3/gameContinueBtn.png");
    gameContinueBtn->setStyleSheet("QPushButton{border:0px;}");
    gameContinueBtn->setIcon(pix);
    gameContinueBtn->setIconSize(QSize(pix.width(),pix.height()));
    gameContinueBtn->move(50,-300);
    //设置存档按钮
    fileBtn = new QPushButton();
    fileBtn->setParent(this);
    fileBtn->setFixedSize(300,150);
    pix.load(":/resource3/copyFile.png");
    fileBtn->setStyleSheet("QPushButton{border:0px;}");
    fileBtn->setIcon(pix);
    fileBtn->setIconSize(QSize(pix.width(),pix.height()));
    fileBtn->move(50,-300);
}
void SinglePlayScene::paintEvent(QPaintEvent *event)//绘制背景图片以及标题等
{
    //绘制背景图片
    QPainter* painter = new QPainter(this);
    QPixmap pix;
    pix.load(":/resource2/PlaySceneBackground.png");
    painter->drawPixmap(0,0,this->width(),this->height(),pix);

    //绘制英文标题
    pix.load(":/resource/QLinkEnglishTitle.png");
    painter->drawPixmap(20,15,pix.width(),pix.height(),pix);
    painter->end();
}

void SinglePlayScene::setBackground()//绘制方块的背景小方框并生成随机数对gameArray进行赋值 初始化配对成功和配对失败图片 初始化倒计时 初始化时钟
{
    int XMax = this->maxX;
    int YMax = this->maxY;
    //设置背景小方框
    for(int i = 0;i < XMax;i++){
        for(int j = 0;j < YMax;j++){
            QLabel * label = new QLabel;
            label->setGeometry(0,0,80,80);
            label->setPixmap(QPixmap(":/resource3/BoardNode.png"));
            label->setParent(this);
            label->move(220 + i * 80,200 + j * 80);
        }
    }
    //通过随机数生成，使gameArray当中的值生成
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i = 1;i < XMax - 1;i++){
        for(int j = 1;j < YMax - 1;j++){
            gameArray[i][j] = qrand()%6+1;
        }
    }
    for(int j = 0; j < YMax;j++){
        gameArray[0][j] = 0;
        gameArray[XMax - 1][j] = 0;
    }
    for(int i = 0; i < XMax;i++){
        gameArray[i][0] = 0;
        gameArray[i][YMax - 1] = 0;
    }
    //初始化相关图片
    this->setPictures();
    //初始化相关标签
    this->setLabels();
}

void SinglePlayScene::setPictures()//初始化相关图片
{
    //初始化配对成功图片和配对失败图片
    this->succeedLabel = new QLabel;
    QPixmap succeedPix;
    succeedPix.load(":/resource3/succeedImg.png");
    succeedLabel->setGeometry(0,0,succeedPix.width(),succeedPix.height());
    succeedLabel->setPixmap(succeedPix);
    succeedLabel->setParent(this);
    succeedLabel->move(1800,-succeedPix.height());
    //初始化游戏结束图片
    this->gameOverLabel = new QLabel;
    QPixmap gameOverPix;
    gameOverPix.load(":/resource3/gameOverBtn.png");
    gameOverLabel->setGeometry(0,0,gameOverPix.width(),gameOverPix.height());
    gameOverLabel->setPixmap(gameOverPix);
    gameOverLabel->setParent(this);
    gameOverLabel->move(-gameOverPix.width(),-gameOverPix.height());
    this->failLabel = new QLabel;
    QPixmap failPix;
    failPix.load(":/resource3/failImg.png");
    failLabel->setGeometry(0,0,failPix.width(),failPix.height());
    failLabel->setPixmap(failPix);
    failLabel->setParent(this);
    failLabel->move(1800,-failPix.height());
}

void SinglePlayScene::setLabels()//初始化相关标签
{
    //初始化时间标签
    this->timeLabel = new QLabel;
    timeLabel->setParent(this);
    timeLabel->setFixedSize(700,200);
    QFont font1;
    font1.setFamily("楷体");
    font1.setPointSize(40);
    timeLabel->setFont(font1);
    timeLabel->setText(QString("剩余时间：%1").arg(this->allTime));
    timeLabel->move(400,0);
    //初始化计时器
    timer = new QTimer(this);
    timer->start(1000);
    //初始化分数标签
    this->scoreLabel = new QLabel;
    scoreLabel->setParent(this);
    scoreLabel->setFixedSize(700,200);
    QFont font2;
    font2.setFamily("仿宋");
    font2.setPointSize(40);
    scoreLabel->setFont(font2);
    scoreLabel->setText("当前得分: 0 分");
    scoreLabel->move(1000,0);
    //初始化有无解标签
    this->haveAnswerLabel = new QLabel;
    haveAnswerLabel->setParent(this);
    haveAnswerLabel->setFixedSize(700,200);
    QFont font3;
    font3.setFamily("楷体");
    font3.setPointSize(40);
    haveAnswerLabel->setFont(font3);
    haveAnswerLabel->setText(QString("有解"));
    haveAnswerLabel->move(1600,0);
}
void SinglePlayScene::setPlayBtn()//绘制连连看按钮函数
{
    int XMax = this->maxX;
    int YMax = this->maxY;
    for(int i = 0;i < XMax;i++){
        for(int j = 0;j < YMax;j++){
            if(gameArray[i][j] == 0 || gameArray[i][j] == 99){
                playBtn[i][j] = new PlayButton();
                playBtn[i][j]->posX = i;
                playBtn[i][j]->posY = j;
                playBtn[i][j]->setParent(this);
                playBtn[i][j]->move(220 + i * 80,200 + j * 80);
            }
            else{
                QString str1 = QString(":/resource3/PlayBtn000%1.jpg").arg(gameArray[i][j]);
                QString str2 = QString(":/resource3/PlayCheckBtn000%1.jpg").arg(gameArray[i][j]);
                //            qDebug()<<str1;
                playBtn[i][j] = new PlayButton(str1,str2);
                playBtn[i][j]->posX = i;
                playBtn[i][j]->posY = j;
                playBtn[i][j]->setParent(this);
                playBtn[i][j]->move(220 + i * 80,200 + j * 80);
            }
        }
    }
}

void SinglePlayScene::updatePlayerLocation()//此函数用于更新玩家位置
{
    this->gameArray[player->x][player->y] = 99;
    player->move(220 + (player->x) * 80,200 + (player->y) * 80);
}

void SinglePlayScene::keyPressEvent(QKeyEvent *event)//重写键盘按下函数
{
    if(event->key() == Qt::Key_P){
        if(this->keyLocked){
            this->gameContinue();
        }
        else{
            this->gameStop();
        }
    }
    //如果键盘被锁定了，那么不进行键盘操作
    if(this->keyLocked)
        return;
    event->accept();
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
        this->keyPressA();
    }
    if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        this->keyPressD();
    }
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        this->keyPressW();
    }
    if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        this->keyPressS();
    }
}
void SinglePlayScene::keyPressS()//键盘按压S时的解决函数
{
    if(this->player->y == maxY-1){
        emit player->playerCannotMove();
        return;
    }
    switch(this->gameArray[player->x][player->y + 1]){
    case 0:{
        this->gameArray[player->x][player->y] = 0;
        this->player->y++;
        this->updatePlayerLocation();
        break;
    }
    default:
        this->solveChecked(this->gameArray[player->x][player->y + 1],player->x,player->y + 1);
    }
}
void SinglePlayScene::keyPressW()//键盘按压W时的解决函数
{
    if(this->player->y == 0){
        emit player->playerCannotMove();
        return;
    }
    switch(this->gameArray[player->x][player->y - 1]){
    case 0:{
        this->gameArray[player->x][player->y] = 0;
        this->player->y--;
        this->updatePlayerLocation();
        break;
    }
    default:
        this->solveChecked(this->gameArray[player->x][player->y - 1],player->x,player->y - 1);
    }
}
void SinglePlayScene::keyPressD()//键盘按压D时的解决函数
{
    if(this->player->x == maxX - 1){
        emit this->player->playerCannotMove();
        return;
    }
    switch(this->gameArray[player->x + 1][player->y]){
    case 0:{
        this->gameArray[player->x][player->y] = 0;
        this->player->x++;
        this->updatePlayerLocation();
        break;
    }
    default:
        this->solveChecked(this->gameArray[player->x + 1][player->y],player->x + 1,player->y);
    }
}
void SinglePlayScene::keyPressA()//键盘按压A时的解决函数
{
    if(player->x == 0){
        emit player->playerCannotMove();
        return;
    }
    switch(this->gameArray[player->x - 1][player->y]){
    case 0:{
        this->gameArray[player->x][player->y] = 0;
        this->player->x--;
        this->updatePlayerLocation();
        break;
    }
    default:
        this->solveChecked(this->gameArray[player->x - 1][player->y],player->x - 1,player->y);
    }
}

void SinglePlayScene::solveChecked(int i,int x,int y)//第一个参数为gameArray数组中的参数 第二个参数和第三个参数为锁定btn的横纵坐标
{
    this->smallPlayBtn(x,y);
    if(i > 0 && i < 7){//非特殊状况
        this->solveNormal(i,x,y);
    }

    if(i == 7){//表示锁定目标为+1s按钮
        this->solve7(i,x,y);
    }

    if(i == 8){//表示锁定目标为shuffle按钮
        if(this->btnInfo[0] == 0){//说明这是第一个被锁定的btn
            this->btnInfo[0] = 1;
            this->btnInfo[3] = i;
            this->btnInfo[1] = x;
            this->btnInfo[2] = y;
        }
        else {
            if(this->btnInfo[3] == 8 && this->btnInfo[1] == x && this->btnInfo[2] == y){
                noPlayBtn(x,y);
                this->shuffle();
                emit player->succeedMatch();
            }
            else{
                emit player->wrongMatch();
                this->largePlayBtn(x,y);
                this->largePlayBtn(btnInfo[1],btnInfo[2]);
            }
            btnInfo[0] = btnInfo[1] = btnInfo[2] = btnInfo[3] = 0;
        }
    }
    if(i == 9){//表示锁定目标为flash按钮
        this->solve9(i,x,y);
    }
}
void SinglePlayScene::solve9(int i, int x, int y)//在i=9的情况下的处理函数
{
    if(this->btnInfo[0] == 0){//说明这是第一个被锁定的btn
        this->btnInfo[0] = 1;
        this->btnInfo[3] = i;
        this->btnInfo[1] = x;
        this->btnInfo[2] = y;
    }
    else {
        if(this->btnInfo[3] == 9 && this->btnInfo[1] == x && this->btnInfo[2] == y){
            this->flashCanBeUsed = true;
            noPlayBtn(x,y);
            emit player->succeedMatch();
        }
        else{
            emit player->wrongMatch();
            this->largePlayBtn(x,y);
            this->largePlayBtn(btnInfo[1],btnInfo[2]);
        }
        btnInfo[0 ]= btnInfo[1] = btnInfo[2] = btnInfo[3] = 0;
    }
}
void SinglePlayScene::solve7(int i, int x, int y)//在i=7的情况下的处理函数
{
    if(this->btnInfo[0] == 0){//说明这是第一个被锁定的btn
        this->btnInfo[0] = 1;
        this->btnInfo[3] = i;
        this->btnInfo[1] = x;
        this->btnInfo[2] = y;
    }
    else {
        if(this->btnInfo[3] == 7 && this->btnInfo[1] == x && this->btnInfo[2] == y){
            this->allTime += 30;
            noPlayBtn(x,y);
            emit player->succeedMatch();
        }
        else{
            emit player->wrongMatch();
            this->largePlayBtn(x,y);
            this->largePlayBtn(btnInfo[1],btnInfo[2]);
        }
        btnInfo[0] = btnInfo[1] = btnInfo[2] = btnInfo[3] = 0;
    }
}
void SinglePlayScene::solveNormal(int i, int x, int y)//在i<7&&i>0情况下的处理函数
{
    if(this->btnInfo[0] == 0){//说明这是第一个被锁定的btn
        this->btnInfo[0] = 1;
        this->btnInfo[3] = i;
        this->btnInfo[1] = x;
        this->btnInfo[2] = y;
    }
    else{
        if(isSatisfied(btnInfo[1],btnInfo[2],x,y)){
            //如果配对成功 那么分数++，并更新
            this->scoreLabel->setText(QString("目前得分:%1 分").arg(++this->score));
            emit player->succeedMatch();
            noPlayBtn(x,y);
            noPlayBtn(btnInfo[1],btnInfo[2]);

            //查询该界面是否有解，若无解，则提示无解，并锁定键盘
            if(!this->gameCanBeSolved()){
                this->keyLocked = true;
                QPalette pe;
                pe.setColor(QPalette::WindowText,Qt::red);
                this->haveAnswerLabel->setPalette(pe);
                this->haveAnswerLabel->setText("无解");
                timer->stop();
                this->gameOverLabel->move(0.5 * (this->width() - gameOverLabel->width()),0.5 * (this->height() - gameOverLabel->height()));
                this->gameOverLabel->raise();
            }
        }
        else{
            emit player->wrongMatch();
            this->largePlayBtn(x,y);
            this->largePlayBtn(btnInfo[1],btnInfo[2]);
        }
        btnInfo[0] = btnInfo[1] = btnInfo[2] = btnInfo[3] = 0;
    }
}

void SinglePlayScene::smallPlayBtn(int x, int y)//该函数用于在锁定btn后将该btn图片变小 参数为锁定btn的横纵坐标
{
    this->playBtn[x][y]->isChecked = true;
    this->playBtn[x][y]->setImage();

}
void SinglePlayScene::largePlayBtn(int x, int y)//该函数用于在解除锁定btn后将该btn图片变小 参数为锁定btn的横纵坐标
{
    this->playBtn[x][y]->isChecked = false;
    this->playBtn[x][y]->setImage();
}

void SinglePlayScene::noPlayBtn(int x, int y)//该函数用于在锁定btn后将该btn图片清除 参数为锁定btn的横纵坐标
{
    this->gameArray[x][y] = 0;
    this->playBtn[x][y]->isChecked = false;
    this->playBtn[x][y]->setIcon(QIcon());
    delete playBtn[x][y];
    this->playBtn[x][y] = new PlayButton();
    playBtn[x][y]->posX = x;
    playBtn[x][y]->posY = y;
    playBtn[x][y]->setParent(this);
    playBtn[x][y]->move(220 + x * 80,200 + y * 80);
    playBtn[x][y]->show();

    connect(playBtn[x][y],&QPushButton::clicked,[=](){
        //如果此时flash不可使用，发出错误信号
        if(!flashCanBeUsed){
            emit player->wrongMatch();
        }
        else{
            //检查该位置是否为空
            if(gameArray[x][y] != 0){
                emit player->wrongMatch();
            }
            else{
                //否则进行角色移动
                gameArray[player->x][player->y] = 0;
                player->x = x;
                player->y = y;
                updatePlayerLocation();
                flashCanBeUsed = false;
            }
        }});

}

void SinglePlayScene::solveSignals()//此函数用于处理信号和槽
{
    connect(player,&Player::wrongMatch,[=](){//处理配对成功/失败的信号和槽
        this->failLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->failLabel->move(1800,-failLabel->height());
        });
    });
    connect(player,&Player::succeedMatch,[=](){
        this->succeedLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->succeedLabel->move(1800,-succeedLabel->height());
        });
    });
    //与时间相关的信号和槽处理工作
    this->solveTimeSignals();
    //触发shuffle技能信号后，试行信号操作
    connect(this,&SinglePlayScene::shuffleShow,[=](){
        this->shuffleSolve();
    });
    //触发flash技能信号后，试行信号操作
    connect(this,&SinglePlayScene::flashShow,[=](){
        this->flashSolve();
    });
    //点击游戏暂停按钮，试行游戏暂停操作
    connect(this->gameStopBtn,&QPushButton::clicked,[=](){
        this->gameStop();
    });
    //点击游戏继续按钮，试行游戏继续操作
    connect(this->gameContinueBtn,&QPushButton::clicked,[=](){
        this->gameContinue();
    });
    this->flash();
    //点击存档按钮，试行存档操作
    connect(this->fileBtn,&QPushButton::clicked,[=](){
        this->getFiled();
    });
}

void SinglePlayScene::solveTimeSignals()
{
    connect(timer,&QTimer::timeout,[=](){//实现计时器功能
        this->allTime--;
        QString str = QString("剩余时间：%1").arg(this->allTime);
        this->timeLabel->setText(str);
        if(this->allTime == 0){//如果时间到0，那么计时停止，label显示时间到，并且无法在对按键进行操作，展示gameOverBtn
            timer->stop();
            this->timeLabel->setText("时间到！");
            this->keyLocked = true;
            this->gameOverLabel->move(0.5 * (this->width() - gameOverLabel->width()),0.5 * (this->height() - gameOverLabel->height()));
            this->gameOverLabel->raise();
        }
        if(this->allTime % 30 == 0){
            emit this->timeAdd();
        }
        if((this->allTime - 3) % 30 == 0){
            emit this->shuffleShow();
        }
        if((this->allTime - 6) % 30 == 0){
            emit this->flashShow();
        }
    });
    //触发增加时间信号后，试行信号操作
    connect(this,&SinglePlayScene::timeAdd,[=](){
        this->addTime();
    });
}
int SinglePlayScene::max(int i, int j)//此函数用于给出两整数的最大值
{
    if(i >= j){
        return i;
    }
    else{
        return j;
    }
}

int SinglePlayScene::min(int i, int j)//此函数用于给出两整数的最小值
{
    if(i >= j){
        return j;
    }
    else{
        return i;
    }
}

bool SinglePlayScene::isSatisfied(int x1,int y1,int x2 ,int y2)//该函数用于判断两点间是否可以用两次及以内折线连接 参数分别为两点的横纵坐标 若返回值为true 表示可以用要求的折线连接 若为false 则不能
{
    if(x1 == x2 && y1 == y2){
        return false;
    }
    if(this->gameArray[x1][y1] == this->gameArray[x2][y2]){
        int xMax = this->maxX;
        int yMax = this->maxY;
        int max = 0;
        int min = 0;
        int i = 0;
        //如果先左右移动 再上下移动 再左右移动
        for(i = 0;i < xMax;i++){
            bool flag = true;
            max = this->max(i,x1);
            min = this->min(i,x1);
            for(int j = min + 1;j < max;j++){
                if(gameArray[j][y1] != 0 && gameArray[j][y1] != 99){
                    flag = false;
                }
            }
            if(i != x1 && gameArray[i][y1] != 0 && gameArray[i][y1] != 99){
                flag = false;
            }

            max = this->max(y1,y2);
            min = this->min(y1,y2);
            for(int j = min + 1;j < max;j++){
                if(gameArray[i][j] != 0 && gameArray[i][j] != 99){
                    flag = false;
                }
            }

            max = this->max(i,x2);
            min = this->min(i,x2);
            for(int j = min + 1;j < max;j++){
                if(gameArray[j][y2] != 0 && gameArray[j][y2] != 99){
                    flag = false;
                }
            }
            if(i != x2 && gameArray[i][y2] != 0 && gameArray[i][y2] != 99){
                flag = false;
            }

            if(flag){
                return true;
            }
        }
        //如果先上下 后左右 再上下移动
        for(int i = 0;i < yMax;i++){
            bool flag = true;
            min = this->min(i,y1);
            max = this->max(i,y1);
            for(int j = min + 1;j < max;j++){
                if(gameArray[x1][j] != 0 && gameArray[x1][j] != 99){
                    flag = false;
                }
            }
            if(i != y1 && gameArray[x1][i] !=0 && gameArray[x1][i] != 99){
                flag = false;
            }
            min = this->min(x2,x1);
            max = this->max(x2,x1);
            for(int j = min + 1;j < max;j++){
                if(gameArray[j][i] != 0 && gameArray[j][i] != 99){
                    flag = false;
                }
            }

            min = this->min(i,y2);
            max = this->max(i,y2);
            for(int j = min + 1;j < max;j++){
                if(gameArray[x2][j] != 0 && gameArray[x2][j] != 99){
                    flag = false;
                }
            }
            if(i != y2 && gameArray[x2][i] != 0 && gameArray[x2][i] != 99){
                flag = false;
            }
            if(flag){
                return true;
            }
        }
    }
    return false;
}

bool SinglePlayScene::haveBtn(int x, int y)//该函数用于判断某位置是否有方块 参数为横纵坐标 返回值为是/否
{
    switch (gameArray[x][y]) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    {
        return true;
        break;
    }
    default:
        return false;
    }
}

bool SinglePlayScene::gameCanBeSolved()//该函数用于判断当前界面是否有解
{
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j<this->maxY;j++){
            if(haveBtn(i,j)){
                //如果当前有btn，那么就去找其他有没有有解的
                for(int x = 0;x < this->maxX;x++){
                    for(int y = 0;y < this->maxY;y++){
                        if(haveBtn(x,y)){
                            if(isSatisfied(i,j,x,y)){
                                return true;
                            }
                        }
                    }
                }
            }

        }
    }
    return false;
}



void SinglePlayScene::addTime()//本函数用于+1s技能的释放
{
    int xTime,yTime;
    if(this->keyLocked){
        return;
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    xTime = qrand() % this->maxX;
    yTime = qrand() % this->maxY;
    if(this->gameArray[xTime][yTime] == 0){
        this->gameArray[xTime][yTime] = 7;
        //        qDebug()<<xTime<<" "<<yTime<<" success";
        QString str1 = ":/resource3/PlayBtn0007.jpg";
        QString str2 = ":/resource3/PlayCheckBtn0007.jpg";
        playBtn[xTime][yTime] = new PlayButton();
        playBtn[xTime][yTime]->setParent(this);
        playBtn[xTime][yTime]->normalImgPath = str1;
        playBtn[xTime][yTime]->checkImgPath = str2;
        playBtn[xTime][yTime]->posX = xTime;
        playBtn[xTime][yTime]->posY = yTime;
        playBtn[xTime][yTime]->setImage();
        playBtn[xTime][yTime]->move(220 + 80 * xTime,200 + 80 * yTime);
        playBtn[xTime][yTime]->show();
        return;
    }
}


void SinglePlayScene::gameStop()//该函数用于使得游戏暂停
{
    //功能包括 锁定键盘 显示“继续游戏”按钮
    this->keyLocked = true;
    this->gameContinueBtn->move(50,1100);
    this->gameStopBtn->move(-50,-300);
    timer->stop();
    this->fileBtn->move(400,1100);
}

void SinglePlayScene::gameContinue()//该函数用于使得游戏继续
{
    //功能包括 解锁键盘 显示“暂停游戏”按钮
    this->keyLocked = false;
    this->gameStopBtn->move(50,1100);
    this->gameContinueBtn->move(-50,-300);
    timer->start();
    this->fileBtn->move(-300,1100);
}

void SinglePlayScene::shuffleSolve()//用于对shuffle技能的处理
{
    int xTime,yTime;

    if(this->keyLocked){
        return;
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    xTime = qrand()%this->maxX;
    yTime = qrand()%this->maxY;
    if(this->gameArray[xTime][yTime] == 0){
        this->gameArray[xTime][yTime] = 8;
        QString str1 = ":/resource3/PlayBtn0008.jpg";
        QString str2 = ":/resource3/PlayCheckBtn0008.jpg";
        playBtn[xTime][yTime] = new PlayButton();
        playBtn[xTime][yTime]->setParent(this);
        playBtn[xTime][yTime]->normalImgPath = str1;
        playBtn[xTime][yTime]->checkImgPath = str2;
        playBtn[xTime][yTime]->posX = xTime;
        playBtn[xTime][yTime]->posY = yTime;
        playBtn[xTime][yTime]->setImage();
        playBtn[xTime][yTime]->move(220 + 80 * xTime,200 + 80 * yTime);
        playBtn[xTime][yTime]->show();
        return;
    }
}

void SinglePlayScene::flashSolve()//用于对flash技能的处理
{
    int xTime,yTime;

    if(this->keyLocked){
        return;
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    xTime = qrand()%this->maxX;
    yTime = qrand()%this->maxY;
    if(this->gameArray[xTime][yTime] == 0){
        this->gameArray[xTime][yTime] = 9;
        QString str1 = ":/resource3/PlayBtn0009.jpg";
        QString str2 = ":/resource3/PlayCheckBtn0009.jpg";
        playBtn[xTime][yTime] = new PlayButton();
        playBtn[xTime][yTime]->setParent(this);
        playBtn[xTime][yTime]->normalImgPath = str1;
        playBtn[xTime][yTime]->checkImgPath = str2;
        playBtn[xTime][yTime]->posX = xTime;
        playBtn[xTime][yTime]->posY = yTime;
        playBtn[xTime][yTime]->setImage();
        playBtn[xTime][yTime]->move(220 + 80 * xTime,200 + 80 * yTime);
        playBtn[xTime][yTime]->show();
        return;
    }
}


void SinglePlayScene::shuffle()//本函数用于对目前界面上所有的方块进行重新排布
{
    int numOfBtn[10]={0};
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
            if(gameArray[i][j] != 99){
                numOfBtn[gameArray[i][j]]++;
//                if(gameArray[i][j] != 0){
                delete playBtn[i][j];
//                }
                gameArray[i][j] = 0;
            }
        }
    }
    QRandomGenerator randomGenerator;
    for(int i = 1;i < 10;i++){
        while(numOfBtn[i] > 0){
            int x = randomGenerator.bounded(0,this->maxX+1);
            int y = randomGenerator.bounded(0,this->maxY+1);
            if(gameArray[x][y] == 0){
                gameArray[x][y] = i;
                numOfBtn[i]--;
            }
        }
    }
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
            if(gameArray[i][j] == 0){
                playBtn[i][j] = new PlayButton();
                playBtn[i][j]->posX = i;
                playBtn[i][j]->posY = j;
                playBtn[i][j]->setParent(this);
                playBtn[i][j]->move(220 + i * 80,200 + j * 80);
                playBtn[i][j]->show();
            }
            else{
                if(gameArray[i][j]!=99){
                    QString str1 = QString(":/resource3/PlayBtn000%1.jpg").arg(gameArray[i][j]);
                    QString str2 = QString(":/resource3/PlayCheckBtn000%1.jpg").arg(gameArray[i][j]);
                    playBtn[i][j] = new PlayButton(str1,str2);
                    playBtn[i][j]->posX = i;
                    playBtn[i][j]->posY = j;
                    playBtn[i][j]->setParent(this);
                    playBtn[i][j]->move(220 + i * 80,200 + j * 80);
                    playBtn[i][j]->show();
                }
            }
        }
    }
    this->flash();
}

void SinglePlayScene::flash()//在flash功能的情况下 点击空白按钮可以进行人物转移
{
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
            connect(playBtn[i][j],&QPushButton::clicked,[=](){
                //如果此时flash不可使用，发出错误信号
                if(!flashCanBeUsed){
                    emit player->wrongMatch();
                }
                else{
                    //检查该位置是否为空
                    if(gameArray[i][j] != 0){
                        emit player->wrongMatch();
                    }
                    else{
                        //否则进行角色移动
                        gameArray[player->x][player->y] = 0;
                        player->x = i;
                        player->y = j;
                        updatePlayerLocation();
                        flashCanBeUsed = false;
                    }
                }
            });
        }
    }
}

void SinglePlayScene::getFiled()//该函数用于试行存档操作
{
    //首先，创建文档
    QString selectDir = QFileDialog::getExistingDirectory();
    QFile file(this->createFile(selectDir,".txt"));
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"fail";
        return;//打开文件失败
    }
    QTextStream out(&file);
    out<<QString::number(this->playerNum)<<endl;
    out<<QString::number(this->score)<<endl;
    out<<QString::number(this->allTime)<<endl;
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
            out<<QString::number(this->gameArray[i][j])<<endl;
        }
    }
    emit this->player->succeedMatch();
}

QString SinglePlayScene::createFile(const QString path,const QString suffix)//创建文件
{
    QString createTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");//文件名不能用“:”命名
    QString fileName = path + "\\" + createTime + suffix;
    QFile file(fileName);

    if(file.exists()){
        return fileName;
    }
    else{
        // 如果没有此文件，就创建
        file.open(QFile::WriteOnly|QFile::Text|QIODevice::Append);
    }
    return fileName;
}
