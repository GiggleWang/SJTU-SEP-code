#include "logictest.h"
#include "singleplayscene.h"
LogicTest::LogicTest(QObject *parent) : QObject(parent)
{

}

//由于单人模式和双人模式下的判定方式完全一致，故此测试以单人模式的代码为例

void LogicTest::case0_testcase()//测试0主要测试判断相同两点时候可以符合条件的情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 0;
        }
    }
    play.gameArray[10][10] = 1;
    play.gameArray[13][8] = 2;
    QVERIFY(play.isSatisfied(13,8,13,8) == false);
}
void LogicTest::case1_testcase()//测试1主要测试两方块并不一样的情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 0;
        }
    }
    play.gameArray[1][1] = 1;
    play.gameArray[20][10] = 2;
    QVERIFY(play.isSatisfied(1,1,20,10) == false);

}
void LogicTest::case2_testcase()//测试2主要测试普通情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 1;
        }
    }
    QVERIFY(play.isSatisfied(1,1,20,10) == false);

}



//测试3-5主要测试判断“先左右移动 再上下移动 再左右移动”的情况
//为了达到测试效果，我们设置3个测试，分别为普通情况以及涉及边界的情形
void LogicTest::case3_testcase()//第3个测试为普通情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 2;i < 7;i++)
        play.gameArray[i][0] = 0;
    for(int i = 0;i < 9;i++)
        play.gameArray[6][i] = 0;
    for(int i = 6;i < 15;i++)
        play.gameArray[i][8] = 0;
    QVERIFY(play.isSatisfied(3,0,14,8) == true);
}

void LogicTest::case4_testcase()//第4个测试是边界情况，即无需进行左右移动，直接进行上下移动后左右移动，测试是否符合
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 2;i < 7;i++)
        play.gameArray[i][0] = 0;
    for(int i = 0;i < 9;i++)
        play.gameArray[6][i] = 0;
    for(int i = 6;i < 15;i++)
        play.gameArray[i][8] = 0;
    QVERIFY(play.isSatisfied(6,0,13,8) == true);
}

void LogicTest::case5_testcase()//第5个测试是更加复杂的边界情况，即无需进行左右移动，直接进行上下移动，后也无需左右移动，同时在路径中加入player（即赋值为99）测试是否符合
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 2;i < 7;i++)
        play.gameArray[i][0] = 0;
    for(int i = 0;i < 9;i++)
        play.gameArray[6][i] = 0;
    for(int i = 6;i < 15;i++)
        play.gameArray[i][8] = 0;
    play.gameArray[6][1] = 99;
    QVERIFY(play.isSatisfied(6,0,6,8) == true);
}
//测试3-5主要测试判断“先上下移动 再左右移动 再上下移动”的情况
//为了达到测试效果，我们设置3个测试，分别为普通情况以及涉及边界的情形
void LogicTest::case6_testcase()//第6个测试为普通情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 1;i < 4;i++)
        play.gameArray[4][i] = 0;
    for(int i = 4;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    QVERIFY(play.isSatisfied(4,1,14,9) == true);
}

void LogicTest::case7_testcase()//第7个测试是边界情况，即无需进行上下移动，直接进行左右移动后上下移动，测试是否符合
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 3;
        }
    }
    for(int i = 1;i < 4;i++)
        play.gameArray[4][i] = 0;
    for(int i = 4;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    QVERIFY(play.isSatisfied(4,3,14,7) == true);
}

void LogicTest::case8_testcase()//第5个测试是更加复杂的边界情况，即无需进行上下移动，直接进行左右移动，后也无需上下移动，同时在路径中加入player（即赋值为99）测试是否符合
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 4;
        }
    }
    for(int i = 1;i < 4;i++)
        play.gameArray[4][i] = 0;
    for(int i = 4;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    play.gameArray[9][3] = 99;
    QVERIFY(play.isSatisfied(8,3,10,3) == true);
}

// 下面是一些边界情况
void LogicTest::case9_testcase()//测试如果两方块是对角并且player与两者都相邻的情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 1;
        }
    }
    play.gameArray[0][0] = 99;
    QVERIFY(play.isSatisfied(1,0,0,1) == true);
}

void LogicTest::case10_testcase()//测试两方块间出现道具的情况
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 1;i < 4;i++)
        play.gameArray[4][i] = 0;
    for(int i = 4;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    play.gameArray[10][3] = 9;
    QVERIFY(play.isSatisfied(4,1,14,9) == false);
}

void LogicTest::case11_testcase()//单独检查拐点处存在方块无法通过的情况（先左右）1
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 2;i < 6;i++)
        play.gameArray[i][0] = 0;
    for(int i = 1;i < 9;i++)
        play.gameArray[6][i] = 0;
    for(int i = 6;i < 15;i++)
        play.gameArray[i][8] = 0;
    QVERIFY(play.isSatisfied(3,0,14,8) == false);
}

void LogicTest::case12_testcase()//单独检查拐点处存在方块无法通过的情况（先上下）1
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 1;i < 3;i++)
        play.gameArray[4][i] = 0;
    for(int i = 5;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    QVERIFY(play.isSatisfied(4,1,14,9) == false);
}

void LogicTest::case13_testcase()//单独检查拐点处存在方块无法通过的情况（先左右）2
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 2;i < 7;i++)
        play.gameArray[i][0] = 0;
    for(int i = 0;i < 8;i++)
        play.gameArray[6][i] = 0;
    for(int i = 7;i < 15;i++)
        play.gameArray[i][8] = 0;
    QVERIFY(play.isSatisfied(3,0,14,8) == false);
}

void LogicTest::case14_testcase()//单独检查拐点处存在方块无法通过的情况（先上下）2
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++){
        for(int j = 0;j < 11;j++){
            play.gameArray[i][j] = 2;
        }
    }
    for(int i = 1;i < 3;i++)
        play.gameArray[4][i] = 0;
    for(int i = 5;i < 15;i++)
        play.gameArray[i][3] = 0;
    for(int i = 3;i < 10;i++)
        play.gameArray[14][i] = 0;
    play.gameArray[14][3] = 4;
    QVERIFY(play.isSatisfied(4,1,14,9) == false);
}


void LogicTest::case15_testcase()//对if(x1 == x2 && y1 == y2)进行条件组合覆盖1
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 0;
        }
    }
    play.gameArray[10][10] = 1;
    play.gameArray[13][10] = 1;
    QVERIFY(play.isSatisfied(10,10,13,10) == true);
}


void LogicTest::case16_testcase()//对if(x1 == x2 && y1 == y2)进行条件组合覆盖2
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 0;
        }
    }
    play.gameArray[10][10] = 2;
    play.gameArray[10][1] = 2;
    QVERIFY(play.isSatisfied(10,10,10,1) == true);
}


void LogicTest::case17_testcase()//检验三次折线可以成功的连接是否成立
{
    SinglePlayScene play;
    for(int i = 0;i < 22;i++)
    {
        for(int j = 0;j < 11;j++)
        {
            play.gameArray[i][j] = 2;
        }
    }
    for(int j = 1;j < 6;j++)
    {
        play.gameArray[j][1] = 0;
    }
    for(int j = 1;j < 6;j++)
    {
        play.gameArray[5][j] = 0;
    }
    for(int j = 5;j < 7;j++)
    {
        play.gameArray[j][5] = 0;
    }
    for(int j = 5;j < 8;j++)
    {
        play.gameArray[7][j] = 0;
    }

    QVERIFY(play.isSatisfied(1,1,7,7) == false);
}
