#pragma once
#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QWidget>
#include "mybutton.h"
#include "choosescene.h"
#include "singleplayscene.h"
#include "partnerplayscene.h"
#include <QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MenuScene; }
QT_END_NAMESPACE

class MenuScene : public QWidget
{
    Q_OBJECT

public:
    MenuScene(QWidget *parent = nullptr);
    ~MenuScene();
    //画面绘制函数
    void paintEvent(QPaintEvent *event);

    //三个按键
    MyButton *giveGameBtn;
    MyButton *newGameBtn;
    MyButton *getOutGameBtn;

    //一个选择模块画面
    ChooseScene* choose;
    //一个单人游戏界面
    SinglePlayScene * fileSinglePlay;
    //一个双人游戏界面
    PartnerPlayScene * filePartnerPlay;
    //用于存储文件
    QFile* file;

    //载入游戏函数
    void fileGame();
    //单人游戏载入并创建游戏
    void singleFile();
    //双人游戏载入并创建游戏
    void partnerFile();



private:
    Ui::MenuScene *ui;
};
#endif // MENUSCENE_H
