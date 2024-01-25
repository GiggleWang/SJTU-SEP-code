#include "player.h"
#include <QDebug>

Player::Player(QString imgPath)//该函数为玩家类的构造函数，用于设置显示路径以及位置检测
{
    this->imagePath = imgPath;
    this->x = 0;
    this->y = 0;
    this->updateImg();
}

void Player::updateImg()//此函数用于上传玩家图片
{
    QPixmap pix;
    bool ret = pix.load(this->imagePath);
    if(!ret){
        qDebug()<<"图片加载失败";
        return;
    }
    this->setFixedSize(75,75);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

