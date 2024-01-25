#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <QPushButton>

class Player : public QPushButton
{
    Q_OBJECT
public:
    Player(QString imgPath);
    QString imagePath;
    int x;
    int y;
    void updateImg();


signals:
    //在玩家无法移动时触发信号
    void playerCannotMove();
    //在玩家匹配两者错误时触发信号
    void wrongMatch();
    //在玩家匹配两者成功时触发信号
    void succeedMatch();

};

#endif // PLAYER_H
