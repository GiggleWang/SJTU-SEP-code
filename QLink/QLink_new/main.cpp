#include "menuscene.h"

#include <QApplication>

//下面内容为运行主函数
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuScene w;
    w.show();
    return a.exec();
}

////下面内容为测试
//#include "logictest.h"

//QTEST_MAIN(LogicTest)
