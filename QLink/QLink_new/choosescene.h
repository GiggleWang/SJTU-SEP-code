#pragma once
#ifndef CHOOSESCENE_H
#define CHOOSESCENE_H

#include <QWidget>
#include "mybutton.h"
#include "singleplayscene.h"
#include "partnerplayscene.h"

class ChooseScene : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseScene(QWidget *parent = nullptr);

    //画面绘制函数
    void paintEvent(QPaintEvent *event);

    //三个按键
    MyButton *SingelModuleBtn;
    MyButton *PartnerModuleBtn;
    MyButton *GetBackBtn;

    //单人游戏界面
    SinglePlayScene* singleplay;
    //双人游戏界面
    PartnerPlayScene* partnerplay;



signals:
    //在点击退出按钮时触发信号给上一界面
    void chooseSceneClose();

};

#endif // CHOOSESCENE_H
