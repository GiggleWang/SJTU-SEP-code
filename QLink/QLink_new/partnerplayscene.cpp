#include "partnerplayscene.h"
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
PartnerPlayScene::PartnerPlayScene(QWidget *parent) : QWidget(parent)//该函数为双人模式界面的构造函数
{
    //设置抓取键盘，从而在操控小人时可以使用上下左右键
    this->grabKeyboard();
    setFocusPolicy(Qt::NoFocus);
    //设定固定大小
    this->setFixedSize(2200,1300);
    this->setWindowTitle("双人模式");
    //设置相关按钮
    this->setButtons();
    //如果点击“退出”按钮，回到上一界面
    connect(getBackBtn,&MyButton::released,[=](){
        emit this->partnerPlayClose();
    });
    this->setBackground();
    this->setPlayBtn();
    //初始化player
    player1 = new Player(":/resource3/player1.jpg");
    player1->setParent(this);
    player1->x = 0;
    player1->y = 0;
    player2 = new Player(":/resource3/player2.jpg");
    player2->setParent(this);
    player2->x = this->maxX - 1;
    player2->y = this->maxY - 1;
    this->updatePlayerLocation(player1);
    this->updatePlayerLocation(player2);
    this->solveSignals();
}

void PartnerPlayScene::setButtons()//该函数用于初始化界面中的按钮
{
    //设置返回按钮
    getBackBtn = new MyButton(":/resource/GetBackButton.png");
    getBackBtn->setParent(this);
    getBackBtn->setFixedSize(400,150);
    getBackBtn->move(this->width()-getBackBtn->width() - 200,1100);
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

void PartnerPlayScene::paintEvent(QPaintEvent *event)//绘制背景图片以及标题等
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

void PartnerPlayScene::setBackground()//绘制方块的背景小方框并生成随机数对gameArray进行赋值初始化配对成功和配对失败图片初始化倒计时 初始化时钟
{
    int maxX = this->maxX;
    int maxY = this->maxY;
    //设置背景小方框
    for(int i = 0;i < maxX;i++){
        for(int j = 0;j < maxY;j++){
            QLabel * label = new QLabel;
            label->setGeometry(0,0,80,80);
            label->setPixmap(QPixmap(":/resource3/BoardNode.png"));
            label->setParent(this);
            label->move(220 + i * 80,200 + j * 80);
        }
    }
    //通过随机数生成，使gameArray当中的值生成
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i = 1;i < maxX-1;i++){
        for(int j = 1;j < maxY-1;j++){
            gameArray[i][j] = qrand()%6+1;
        }
    }
    for(int j = 0; j < maxY;j++){
        gameArray[0][j] = 0;
        gameArray[maxX-1][j] = 0;
    }
    for(int i = 0; i < maxX;i++){
        gameArray[i][0] = 0;
        gameArray[i][maxY-1] = 0;
    }
    //初始化相关图片
    this->setPictures();
    //初始化相关标签
    this->setLabels();
}

void PartnerPlayScene::setPictures()//初始化图片的函数
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

void PartnerPlayScene::setLabels()//初始化标签的函数
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
    this->scoreLabel1 = new QLabel;
    scoreLabel1->setParent(this);
    scoreLabel1->setFixedSize(700,200);
    QFont font2;
    font2.setFamily("仿宋");
    font2.setPointSize(30);
    scoreLabel1->setFont(font2);
    scoreLabel1->setText("player1: 0 分");
    scoreLabel1->move(1000,0);
    this->scoreLabel2 = new QLabel;
    scoreLabel2->setParent(this);
    scoreLabel2->setFixedSize(700,200);
    scoreLabel2->setFont(font2);
    scoreLabel2->setText("player2: 0 分");
    scoreLabel2->move(1450,0);
    //初始化有无解标签
    this->haveAnswerLabel = new QLabel;
    haveAnswerLabel->setParent(this);
    haveAnswerLabel->setFixedSize(700,200);
    QFont font3;
    font3.setFamily("楷体");
    font3.setPointSize(40);
    haveAnswerLabel->setFont(font3);
    haveAnswerLabel->setText(QString("有解"));
    haveAnswerLabel->move(30,200);
}
void PartnerPlayScene::setPlayBtn()//本函数用于设置（初始化）游戏方块
{
    int maxX = this->maxX;
    int maxY = this->maxY;
    for(int i = 0;i < maxX;i++){
        for(int j = 0;j < maxY;j++){
            if(gameArray[i][j] == 0||gameArray[i][j] == 99){
                playBtn[i][j]=new PlayButton();
                playBtn[i][j]->posX = i;
                playBtn[i][j]->posY = j;
                playBtn[i][j]->setParent(this);
                playBtn[i][j]->move(220+i*80,200+j*80);
            }
            else{
                QString str1 = QString(":/resource3/PlayBtn000%1.jpg").arg(gameArray[i][j]);
                QString str2 = QString(":/resource3/PlayCheckBtn000%1.jpg").arg(gameArray[i][j]);
                playBtn[i][j] = new PlayButton(str1,str2);
                playBtn[i][j]->posX = i;
                playBtn[i][j]->posY = j;
                playBtn[i][j]->setParent(this);
                playBtn[i][j]->move(220 + i * 80,200 + j * 80);
            }
        }
    }
}

void PartnerPlayScene::updatePlayerLocation(Player *playerTmp)//此函数用于更新玩家位置
{
    this->gameArray[playerTmp->x][playerTmp->y] = 99;
    playerTmp->move(220 + (playerTmp->x) * 80,200 + (playerTmp->y) * 80);
}

void PartnerPlayScene::keyPressEvent(QKeyEvent *event)//重写键盘按下函数
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
    if(event->key() == Qt::Key_A){
        this->keyPressA();
    }
    if(event->key() == Qt::Key_D){
        this->keyPressD();
    }
    if(event->key() == Qt::Key_W){
        this->keyPressW();
    }
    if(event->key() == Qt::Key_S){
        this->keyPressS();
    }

    if(event->key() == Qt::Key_Left)
    {
        this->keyPressLeft();
    }
    if(event->key() == Qt::Key_Right){
        this->keyPressRight();
    }
    if(event->key() == Qt::Key_Up){
        this->keyPressUp();
    }
    if(event->key() == Qt::Key_Down){
        this->keyPressDown();
    }
}
void PartnerPlayScene::keyPressDown()//键盘按压Down时的解决函数
{
    if(this->player2->y == maxY-1){
        emit player2->playerCannotMove();
        return;
    }
    switch(this->gameArray[player2->x][player2->y + 1]){
    case 0:{
        this->gameArray[player2->x][player2->y] = 0;
        this->player2->y++;
        this->updatePlayerLocation(player2);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player2->x][player2->y + 1],player2->x,player2->y + 1,player2);
    }
}
void PartnerPlayScene::keyPressUp()//键盘按压Up时的解决函数
{
    if(this->player2->y == 0){
        emit player2->playerCannotMove();
        return;
    }
    switch(this->gameArray[player2->x][player2->y - 1]){
    case 0:{
        this->gameArray[player2->x][player2->y]=0;
        this->player2->y--;
        this->updatePlayerLocation(player2);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player2->x][player2->y - 1],player2->x,player2->y - 1,player2);
    }
}
void PartnerPlayScene::keyPressRight()//键盘按压Right时的解决函数
{
    if(this->player2->x == maxX-1){
        emit this->player2->playerCannotMove();
        return;
    }
    switch(this->gameArray[player2->x + 1][player2->y]){
    case 0:{
        this->gameArray[player2->x][player2->y] = 0;
        this->player2->x++;
        this->updatePlayerLocation(player2);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player2->x + 1][player2->y],player2->x + 1,player2->y,player2);
    }
}
void PartnerPlayScene::keyPressLeft()//键盘按压Left时的解决函数
{
    if(player2->x == 0){
        emit player2->playerCannotMove();
        return;
    }
    switch(this->gameArray[player2->x-1][player2->y]){
    case 0:{
        this->gameArray[player2->x][player2->y]=0;
        this->player2->x--;
        this->updatePlayerLocation(player2);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player2->x - 1][player2->y],player2->x - 1,player2->y,player2);
    }
}
void PartnerPlayScene::keyPressA()//键盘按压A时的解决函数
{
    if(player1->x == 0){
        emit player1->playerCannotMove();
        return;
    }
    switch(this->gameArray[player1->x-1][player1->y]){
    case 0:{
        this->gameArray[player1->x][player1->y]=0;
        this->player1->x--;
        this->updatePlayerLocation(player1);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player1->x-1][player1->y],player1->x - 1,player1->y,player1);
    }
}
void PartnerPlayScene::keyPressD()//键盘按压D时的解决函数
{
    if(this->player1->x == maxX-1){
        emit this->player1->playerCannotMove();
        return;
    }
    switch(this->gameArray[player1->x + 1][player1->y]){
    case 0:{
        this->gameArray[player1->x][player1->y] = 0;
        this->player1->x++;
        this->updatePlayerLocation(player1);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player1->x+1][player1->y],player1->x+1,player1->y,player1);
    }
}
void PartnerPlayScene::keyPressW()//键盘按压W时的解决函数
{
    if(this->player1->y == 0){
        emit player1->playerCannotMove();
        return;
    }
    switch(this->gameArray[player1->x][player1->y - 1]){
    case 0:{
        this->gameArray[player1->x][player1->y] = 0;
        this->player1->y--;
        this->updatePlayerLocation(player1);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player1->x][player1->y - 1],player1->x,player1->y - 1,player1);
    }
}
void PartnerPlayScene::keyPressS()//键盘按压S时的解决函数
{
    if(this->player1->y == maxY-1){
        emit player1->playerCannotMove();
        return;
    }
    switch(this->gameArray[player1->x][player1->y + 1]){
    case 0:{

        this->gameArray[player1->x][player1->y]=0;
        this->player1->y++;
        this->updatePlayerLocation(player1);
        break;
    }
    default:
        this->solveChecked(this->gameArray[player1->x][player1->y + 1],player1->x,player1->y + 1,player1);
    }
}
void PartnerPlayScene::solveChecked(int i,int x,int y,Player *playerTmp)//第一个参数为gameArray数组中的参数 第二个参数和第三个参数为锁定btn的横纵坐标
{
    this->smallPlayBtn(x,y);
    int *btnInfo = NULL , *score;
    QLabel *scoreLabel;
    if(playerTmp == player1){
        btnInfo = btnInfo1;
        score = &score1;
        scoreLabel = scoreLabel1;
    }
    if(playerTmp == player2){
        btnInfo = btnInfo2;
        score = &score2;
        scoreLabel = scoreLabel2;
    }
    if(i > 0 && i < 7){//非特殊状况
        if(btnInfo[0] == 0){//说明这是第一个被锁定的btn
            btnInfo[0] = 1;
            btnInfo[3] = i;
            btnInfo[1] = x;
            btnInfo[2] = y;
        }
        else{
            if(isSatisfied(btnInfo[1],btnInfo[2],x,y)){//如果配对成功 那么分数++，并更新
                if(playerTmp == player1){
                    scoreLabel->setText(QString("player1:%1 分").arg(++(*score)));
                }
                if(playerTmp == player2){
                    scoreLabel->setText(QString("player2:%1 分").arg(++(*score)));
                }
                emit playerTmp->succeedMatch();
                noPlayBtn(x,y);
                noPlayBtn(btnInfo[1],btnInfo[2]);
                if(!this->gameCanBeSolved()){//查询该界面是否有解，若无解，则提示无解，并锁定键盘
                    this->keyLocked=true;
                    QPalette pe;
                    pe.setColor(QPalette::WindowText,Qt::red);
                    this->haveAnswerLabel->setPalette(pe);
                    this->haveAnswerLabel->setText("无解");
                    timer->stop();
                    this->gameOverLabel->move(0.5*(this->width() - gameOverLabel->width()),0.5*(this->height() - gameOverLabel->height()));
                    this->gameOverLabel->raise();
                }
            }
            else{
                emit playerTmp->wrongMatch();
                this->largePlayBtn(x,y);
                this->largePlayBtn(btnInfo[1],btnInfo[2]);
            }
            btnInfo[0] = btnInfo[1] = btnInfo[2] = btnInfo[3] = 0;
        }
    }
}

void PartnerPlayScene::smallPlayBtn(int x,int y)//该函数用于在锁定btn后将该btn图片变小 参数为锁定btn的横纵坐标
{
    this->playBtn[x][y]->isChecked = true;
    this->playBtn[x][y]->setImage();
}

void PartnerPlayScene::largePlayBtn(int x,int y)//该函数用于在解除锁定btn后将该btn图片变小 参数为锁定btn的横纵坐标
{
    this->playBtn[x][y]->isChecked = false;
    this->playBtn[x][y]->setImage();
}

void PartnerPlayScene::noPlayBtn(int x,int y)//该函数用于在锁定btn后将该btn图片清除 参数为锁定btn的横纵坐标
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
}

bool PartnerPlayScene::isSatisfied(int x1,int y1,int x2,int y2)//该函数用于判断两点间是否可以用两次及以内折线连接
{
    //该函数用于判断两点间是否可以用两次及以内折线连接
    //参数分别为两点的横纵坐标
    //若返回值为true 表示可以用要求的折线连接 若为false 则不能
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
                if(gameArray[j][y1] != 0&&gameArray[j][y1] != 99){
                    flag = false;
                }
            }
            if(i != x1 &&gameArray[i][y1] != 0&&gameArray[i][y1] != 99){
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
                if(gameArray[j][y2] != 0&&gameArray[j][y2] != 99){
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
                if(gameArray[x1][j] != 0&&gameArray[x1][j] != 99){
                    flag = false;
                }
            }
            if(i != y1 && gameArray[x1][i] != 0 && gameArray[x1][i] != 99){
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
            if(i != y2&&gameArray[x2][i]!=0&&gameArray[x2][i]!=99){
                flag = false;
            }
            if(flag){
                return true;
            }
        }
    }
    return false;
}


void PartnerPlayScene::solveSignals()//此函数用于处理信号和槽
{
    connect(player1,&Player::wrongMatch,[=](){//处理配对成功/失败的信号和槽
        this->failLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->failLabel->move(1800,-failLabel->height());
        });
    });
    connect(player1,&Player::succeedMatch,[=](){
        this->succeedLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->succeedLabel->move(1800,-succeedLabel->height());
        });
    });
    connect(player2,&Player::wrongMatch,[=](){
        this->failLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->failLabel->move(1800,-failLabel->height());
        });
    });
    connect(player2,&Player::succeedMatch,[=](){
        this->succeedLabel->move(1800,50);
        QTimer::singleShot(1000,this,[=](){//实现倒计时1秒钟
            this->succeedLabel->move(1800,-succeedLabel->height());
        });
    });
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
    });
    connect(this->gameStopBtn,&QPushButton::clicked,[=](){//点击游戏暂停按钮，试行游戏暂停操作
        this->gameStop();
    });
    connect(this->gameContinueBtn,&QPushButton::clicked,[=](){//点击游戏继续按钮，试行游戏继续操作
        this->gameContinue();
    });
    connect(this->fileBtn,&QPushButton::clicked,[=](){//点击存档按钮，试行存档操作
        this->getFiled();;
    });
}

int PartnerPlayScene::max(int i, int j)//此函数用于给出两整数的最大值
{
    if(i >= j){
        return i;
    }
    else{
        return j;
    }
}

int PartnerPlayScene::min(int i, int j)//此函数用于给出两整数的最小值
{
    if(i >= j){
        return j;
    }
    else{
        return i;
    }
}
bool PartnerPlayScene::haveBtn(int x,int y)//该函数用于判断某位置是否有方块 参数为横纵坐标 返回值为是/否
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

void PartnerPlayScene::gameStop()//功能包括 锁定键盘 显示“继续游戏”按钮
{
    this->keyLocked = true;
    this->gameContinueBtn->move(50,1100);
    this->gameStopBtn->move(-50,-300);
    timer->stop();
    this->fileBtn->move(400,1100);
}

void PartnerPlayScene::gameContinue()//功能包括 解锁键盘 显示“暂停游戏”按钮
{
    this->keyLocked = false;
    this->gameStopBtn->move(50,1100);
    this->gameContinueBtn->move(-50,-300);
    timer->start();
    this->fileBtn->move(-300,1100);
}

bool PartnerPlayScene::gameCanBeSolved()//该函数用于判断当前界面是否有解
{
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
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
void PartnerPlayScene::getFiled()//该函数用于试行存档操作
{
    //首先，创建文档
    QString selectDir = QFileDialog::getExistingDirectory();
    //    qDebug() << "Dir Path:" << selectDir;
    QFile file(this->createFile(selectDir,".txt"));
    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"fail";
        return;//打开文件失败
    }

    QTextStream out(&file);
    out<<QString::number(this->playerNum)<<endl;
    out<<QString::number(this->score1)<<endl;
    out<<QString::number(this->score2)<<endl;
    out<<QString::number(this->allTime)<<endl;
    for(int i = 0;i < this->maxX;i++){
        for(int j = 0;j < this->maxY;j++){
            out<<QString::number(this->gameArray[i][j])<<endl;
            if(this->gameArray[i][j] == 99){
                if(i == player1->x && j == player1->y){
                    out<<"player1"<<endl;
                }
                if(i == player2->x && j == player2->y){
                    out<<"player2"<<endl;
                }
            }
        }
    }
    file.close();
    emit this->player1->succeedMatch();
}

QString PartnerPlayScene::createFile(const QString path,const QString suffix)//该函数用于创建文件
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


