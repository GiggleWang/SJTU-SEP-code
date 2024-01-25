#pragma once
#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    //构造函数 第一个参数表示初始显示图片的路径
    MyButton(QString normalPath);

    //存储的初始显示图片
    QString normalPicturePath;

    //按压特效
    //向下跳
    void pressDown();
    //向上抬
    void releaseUp();

    //重写按压函数和抬起函数
    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;


signals:

};

#endif // MYBUTTON_H
