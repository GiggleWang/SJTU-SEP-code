#include "mybutton.h"
#include <QPropertyAnimation>
#include <QDebug>

MyButton::MyButton(QString normalPath)//该函数为自建按钮类的构造函数
{
    normalPicturePath = normalPath;
    QPixmap pix;
    bool flag = pix.load(normalPicturePath);
    if(!flag){
        QString str = QString("图片%1加载错误").arg(normalPicturePath);
        return;
    }
    //设置图片的固定大小
    this->setFixedSize(pix.width(),pix.height());
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //将图片贴到button上
    this->setIcon(pix);
    //设置图片的大小
    this->setIconSize(QSize(pix.width(),pix.height()));
}


void MyButton::pressDown()//处理鼠标按下时的动画
{
    //创建动态对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    //设置经历时间
    animation->setDuration(50);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y() + 10,this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

void MyButton::releaseUp()//处理鼠标抬起时的动画
{
    //创建动态对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    //设置经历时间
    animation->setDuration(50);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y() - 10,this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

void MyButton::mousePressEvent(QMouseEvent *event)//重写鼠标按下函数
{
    pressDown();
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent *event)//重写鼠标抬起函数
{
    releaseUp();
    QPushButton::mouseReleaseEvent(event);
}
