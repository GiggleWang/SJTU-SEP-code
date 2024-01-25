#pragma once
#ifndef SINGLEPLAYSCENE_H
#define SINGLEPLAYSCENE_H

#include <QWidget>
#include "mybutton.h"
#include "playbutton.h"
#include "player.h"
#include <QLabel>

class SinglePlayScene : public QWidget
{
    Q_OBJECT
public:
    explicit SinglePlayScene(QWidget *parent = nullptr);
    int maxX=22;
    int maxY=11;
    //存储各方块情况的数组
    // 如果值为0 表示空 即未有占用
    // 如果值为1-11 表示被第1-11号图片占据
    // 如果值为99 表示被player占据
    // 如果值为11 表示为+1s技能
    // 如果值为11 表示为shuffle技能
    // 如果值为9 表示为flash技能
    int gameArray[22][11];
    //画面中的连连看按钮
    PlayButton* playBtn[22][11];
    int playerNum=1;
    //退出按钮
    MyButton * getBackBtn;
    //继续游戏按钮
    QPushButton * gameContinueBtn;
    //暂停游戏按钮
    QPushButton * gameStopBtn;
    //存档按钮
    QPushButton * fileBtn;
    //玩家
    Player * player;
    //显示配对成功的标志
    QLabel *succeedLabel;
    //显示配对失败的标志
    QLabel *failLabel;
    //显示游戏结束的标志
    QLabel *gameOverLabel;
    //显示剩余时间的标签
    QLabel *timeLabel;
    //显示所得分数的标签
    QLabel *scoreLabel;
    //显示当前界面是否有解的标签
    QLabel *haveAnswerLabel;
    //所得分数
    int score = 0;
    //关于剩余时间的定时器
    QTimer *timer;
    //设置的一共游戏时间
    int allTime = 100;
    //设置是否锁定键盘无法操作
    bool keyLocked = false;
    //画面绘制函数
    void paintEvent(QPaintEvent *event);
    //设置方块背景函数
    void setBackground();

    //绘制连连看按钮函数
    void setPlayBtn();
    //更新玩家位置
    void updatePlayerLocation();
    //重写键盘按下函数
    void keyPressEvent(QKeyEvent *event);
    //处理锁定目标函数
    void solveChecked(int i,int x,int y);
    //存储第一个锁定的方块信息 分别为 是否是第一个锁定目标（若已有锁定为1） x y坐标 对应的gameArray存储数字
    int btnInfo[4] = {0};
    //在锁定后改变该坐标位置图片样式
    void smallPlayBtn(int x,int y);
    //在解除锁定后改变该坐标位置图片样式
    void largePlayBtn(int x,int y);
    //在消除后清除图层
    void noPlayBtn(int x,int y);
    //判断锁定的两个方块是否符合条件
    bool isSatisfied(int x1,int y1,int x2,int y2);
    //判断该界面是否有解
    bool gameCanBeSolved();
    //用于处理相关信号和槽的函数
    void solveSignals();
    //用于判断该位置是否有方块
    bool haveBtn(int x,int y);
    //用于判断是否可以使用flash功能
    bool flashCanBeUsed = false;
    //用于比较两值大小的函数
    int max(int i,int j);
    int min(int i,int j);
    //用于初始化相关按钮的函数
    void setButtons();
    //初始化图片的函数
    void setPictures();
    //初始化标签的函数
    void setLabels();
    //用于对+1s技能的释放进行操作
    void addTime();
    //用于进行游戏暂停函数
    void gameStop();
    //用于暂停游戏后继续游戏的函数
    void gameContinue();
    //用于对shuffle技能的处理
    void shuffleSolve();
    //用于对flash技能的处理
    void flashSolve();
    //用于对界面方块进行重排
    void shuffle();
    //用于flash功能的信号连接
    void flash();
    //用于存档操作
    void getFiled();
    //创建文件相关函数
    QString createFile(const QString path,const QString suffix = ".txt");   // 创建.txt文件
    //键盘按压W时的解决函数
    void keyPressW();
    //键盘按压S时的解决函数
    void keyPressS();
    //键盘按压A时的解决函数
    void keyPressA();
    //键盘按压D时的解决函数
    void keyPressD();
    //在i<7&&i>0情况下的处理函数
    void solveNormal(int i, int x,int y);
    //在i=7的情况下的处理函数
    void solve7(int i,int x,int y);
    //在i=9的情况下的处理函数
    void solve9(int i,int x,int y);
    //对计时器相关功能的信号与槽的连接函数
    void solveTimeSignals();

signals:
    //在点击退出按钮时触发信号给上一界面
    void singlePlayClose();
    //发出触发+1s技能的信号
    void timeAdd();
    //发出触发shuffle技能信号
    void shuffleShow();
    //发出触发flash技能信号
    void flashShow();
};

#endif // SINGLEPLAYSCENE_H
