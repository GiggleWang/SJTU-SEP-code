#include "menuscene.h"
#include "ui_menuscene.h"
#include <QPainter>

#include <QDebug>


MenuScene::MenuScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuScene)//该函数为菜单界面的构造函数
{
    ui -> setupUi(this);
    //设置固定大小
    this -> setFixedSize(2200,1300);
    //设置标题
    this -> setWindowTitle("游戏菜单");
    //设置主菜单中的三个按键
    newGameBtn = new MyButton(":/resource/NewGameButton.png");
    newGameBtn->setParent(this);
    newGameBtn->setFixedSize(400,150);
    newGameBtn->move(0.5*(this->width() - newGameBtn->width()),600);

    giveGameBtn = new MyButton(":/resource/GiveGameButton.png");
    giveGameBtn->setParent(this);
    giveGameBtn->setFixedSize(400,150);
    giveGameBtn->move(0.5*(this->width() - newGameBtn->width()),800);

    getOutGameBtn = new MyButton(":/resource/GetOutGameButton.png");
    getOutGameBtn->setParent(this);
    getOutGameBtn->setFixedSize(400,150);
    getOutGameBtn->move(0.5*(this->width() - newGameBtn->width()),1000);
    //一点击“退出游戏”按钮，退出游戏
    connect(getOutGameBtn,&QPushButton::released,[=](){
        this->close();
    });
    //一点击“新游戏”按钮，进入选择模式模块
    connect(newGameBtn,&QPushButton::released,[=](){
        //初始化choose界面
        choose = new ChooseScene();
        this->hide();
        choose->show();
        //如果有信号表示选择模式界面关闭 则打开此界面
        connect(choose,&ChooseScene::chooseSceneClose,[=]{
            this->show();
            choose->hide();
        });
    });

    //点击“载入游戏”后，进入载入游戏函数
    connect(giveGameBtn,&QPushButton::clicked,[=](){
        this->fileGame();

    });
}

MenuScene::~MenuScene()//该函数为菜单界面的析构函数
{
    delete ui;
}

void MenuScene::paintEvent(QPaintEvent *event)//绘制背景图片以及标题等
{
    //绘制背景图片
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/SetUpBackground.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //绘制英文标题和中文标题
    pix.load(":/resource/QLinkEnglishTitle.png");
    painter.drawPixmap(50,50,pix.width(),pix.height(),pix);
    pix.load(":/resource/QLinkChineseTitle.png");
    painter.drawPixmap(0.5 * (this->width() - pix.width()),pix.height() * 0.3,pix.width(),pix.height(),pix);

}

void MenuScene::fileGame()
{
    //该函数用于进行选定并处理文件，读取信息
    QFileDialog fileDialog;
    QString str;
    str=fileDialog.getOpenFileName(this,"请选定打开文件","C:/Users/23861/Desktop");
    file=new QFile(str);
    file->open(QIODevice::ReadOnly);
    int playNum=file->readLine().toInt();
    this->hide();
    if(playNum == 1){
        this->singleFile();
        connect(fileSinglePlay,&SinglePlayScene::singlePlayClose,[=]{
            this->show();
            this->fileSinglePlay->close();
        });
    }
    if(playNum == 2){
        this->partnerFile();
        connect(filePartnerPlay,&PartnerPlayScene::partnerPlayClose,[=]{
            this->show();
            this->filePartnerPlay->close();
        });
    }

}


void MenuScene::singleFile()//该函数用于处理单人模式的文件信息提取及展示
{
    this->fileSinglePlay = new SinglePlayScene;
    //先清空所有的playBtn
    for(int i = 0;i < fileSinglePlay->maxX;i++){
        for(int j = 0;j < fileSinglePlay->maxY;j++){
            this->fileSinglePlay->gameArray[i][j] = 0;
            delete this->fileSinglePlay->playBtn[i][j];
        }
    }
    this->fileSinglePlay->setPlayBtn();
    //下面开始读取文件信息
    this->fileSinglePlay->score = file->readLine().toInt();
    this->fileSinglePlay->allTime = file->readLine().toInt();
    for(int i = 0;i < fileSinglePlay->maxX;i++){
        for(int j = 0;j < fileSinglePlay->maxY;j++){
            this->fileSinglePlay->gameArray[i][j] = file->readLine().toInt();
            if(this->fileSinglePlay->gameArray[i][j] == 99){
                this->fileSinglePlay->player->x = i;
                this->fileSinglePlay->player->y = j;
                this->fileSinglePlay->updatePlayerLocation();
            }
        }
    }
    this->fileSinglePlay->setPlayBtn();
    this->fileSinglePlay->flash();//
    QString str = QString("剩余时间：%1").arg(this->fileSinglePlay->allTime);
    this->fileSinglePlay->timeLabel->setText(str);
    this->fileSinglePlay->scoreLabel->setText(QString("目前得分:%1 分").arg(this->fileSinglePlay->score));
    fileSinglePlay->show();
}


void MenuScene::partnerFile()//该函数用于处理双人模式的文件信息提取及展示
{
    this->filePartnerPlay = new PartnerPlayScene;
    //先清空所有的playBtn
    for(int i = 0;i < filePartnerPlay->maxX;i++){
        for(int j = 0;j < filePartnerPlay->maxY;j++){
            this->filePartnerPlay->gameArray[i][j] = 0;
            delete this->filePartnerPlay->playBtn[i][j];
        }
    }
    this->filePartnerPlay->setPlayBtn();
    //下面开始读取文件信息
    this->filePartnerPlay->score1 = file->readLine().toInt();
    this->filePartnerPlay->score2 = file->readLine().toInt();
    this->filePartnerPlay->allTime = file->readLine().toInt();
    for(int i = 0;i < filePartnerPlay->maxX;i++){
        for(int j = 0;j < filePartnerPlay->maxY;j++){
            this->filePartnerPlay->gameArray[i][j] = file->readLine().toInt();
            if(this->filePartnerPlay->gameArray[i][j] == 99){
                qDebug()<<"accessed";
                QString tmpString = file->readLine();
                qDebug()<<tmpString;
                if(tmpString == "player1\n"){
                    this->filePartnerPlay->player1->x = i;
                    this->filePartnerPlay->player1->y = j;
                    this->filePartnerPlay->updatePlayerLocation(this->filePartnerPlay->player1);
                    qDebug()<<"player1"<<i<<" "<<j;
                }
                if(tmpString == "player2\n"){
                    this->filePartnerPlay->player2->x = i;
                    this->filePartnerPlay->player2->y = j;
                    this->filePartnerPlay->updatePlayerLocation(this->filePartnerPlay->player2);
                    qDebug()<<"player2"<<i<<" "<<j;
                }
            }
        }
    }
    this->filePartnerPlay->setPlayBtn();
    QString str = QString("剩余时间：%1").arg(this->filePartnerPlay->allTime);
    this->filePartnerPlay->timeLabel->setText(str);
    this->filePartnerPlay->scoreLabel1->setText(QString("目前得分:%1 分").arg(this->filePartnerPlay->score1));
    this->filePartnerPlay->scoreLabel2->setText(QString("目前得分:%1 分").arg(this->filePartnerPlay->score2));
    filePartnerPlay->show();
}
