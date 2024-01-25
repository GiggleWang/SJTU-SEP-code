#include "choosescene.h"
#include <QPainter>
#include <QDebug>

ChooseScene::ChooseScene(QWidget *parent) : QWidget(parent)//该函数为选择界面的构造函数
{
    //设定固定大小
    this->setFixedSize(2200,1300);
    this->setWindowTitle("选择模式");
    //创建三个按钮
    SingelModuleBtn = new MyButton(":/resource2/SingleModuleBtn.png");
    SingelModuleBtn->setParent(this);
    SingelModuleBtn->setFixedSize(400,150);
    SingelModuleBtn->move(0.5*(this->width()-SingelModuleBtn->width()),700);
    PartnerModuleBtn = new MyButton(":/resource2/PartnerModuleBtn.png");
    PartnerModuleBtn->setParent(this);
    PartnerModuleBtn->setFixedSize(400,150);
    PartnerModuleBtn->move(0.5*(this->width() - PartnerModuleBtn->width()),900);
    GetBackBtn = new MyButton(":/resource/GetBackButton.png");
    GetBackBtn->setParent(this);
    GetBackBtn->setFixedSize(400,150);
    GetBackBtn->move(this->width() - GetBackBtn->width() - 200,1100);
    //如果点击“退出”按钮，回到上一界面
    connect(GetBackBtn,&MyButton::released,[=](){
        emit this->chooseSceneClose();
    });
    //如果点击“单人游戏”按钮，进入单人游戏界面
    //    singleplay=new SinglePlayScene();
    connect(SingelModuleBtn,&QPushButton::clicked,[=](){
        //如果点击“单人游戏”按钮，进入单人游戏界面
        singleplay = new SinglePlayScene();
        this->hide();
        singleplay->show();
        connect(singleplay,&SinglePlayScene::singlePlayClose,[=]{
            this->show();
            singleplay->hide();
        });
    });
    connect(PartnerModuleBtn,&QPushButton::clicked,[=](){
        //如果点击“双人游戏”按钮，进入双人游戏界面
        partnerplay = new PartnerPlayScene();
        this->hide();
        partnerplay->show();
        connect(partnerplay,&PartnerPlayScene::partnerPlayClose,[=](){
            this->show();
            partnerplay->hide();
        });
    });
}


void ChooseScene::paintEvent(QPaintEvent *event)//绘制背景图片以及标题等
{
    //绘制背景图片
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/SetUpBackground.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制英文标题和中文主题
    pix.load(":/resource/QLinkEnglishTitle.png");
    painter.drawPixmap(50,50,pix.width(),pix.height(),pix);
    pix.load(":/resource2/ChooseSceneTitle.png");
    painter.drawPixmap(0.5 * (this->width() - pix.width()),pix.height(),pix.width(),pix.height(),pix);
}
