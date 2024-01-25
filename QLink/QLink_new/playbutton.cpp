#include "playbutton.h"
#include <QDebug>
PlayButton::PlayButton()//该函数为对playbutton无参数情况下的设置
{
    this->setFixedSize(75,75);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(QIcon());
    this->setIconSize(QSize(75,75));
}

PlayButton::PlayButton(QString normalPath,QString checkPath)//该函数为对playbutton有参数情况下的设置 第一个参数为常规情况下的显示路径 第二个参数为被锁定情况下的显示路径
{
    this->normalImgPath = normalPath;
    this->checkImgPath = checkPath;
    this->setImage();
}

void PlayButton::setImage()//此函数旨在将对应的方块图片绘制到画面中
{
    if(!this->isChecked){
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret){
            qDebug()<<"图片加载失败";
            return;
        }

        this->setFixedSize(75,75);
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    else{
        QPixmap pix;
        bool ret = pix.load(this->checkImgPath);
        if(!ret){
            qDebug()<<"图片加载失败";
            return;
        }

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

    }

}
