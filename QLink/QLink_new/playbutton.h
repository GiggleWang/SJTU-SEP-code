#pragma once
#ifndef PLAYBUTTON_H
#define PLAYBUTTON_H

#include <QPushButton>

class PlayButton : public QPushButton
{
    Q_OBJECT
public:
    PlayButton();
    PlayButton(QString normalPath,QString checkPath);
    QString normalImgPath;
    QString checkImgPath;
    int posX;
    int posY;
    bool isChecked = false;
    void setImage();
signals:

};

#endif // PLAYBUTTON_H
