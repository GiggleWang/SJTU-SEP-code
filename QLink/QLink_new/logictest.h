#pragma once
#ifndef LOGICTEST_H
#define LOGICTEST_H

#include <QtTest/QtTest>

class LogicTest : public QObject
{
    Q_OBJECT
private slots:
    void case0_testcase();
    void case1_testcase();
    void case2_testcase();
    void case3_testcase();
    void case4_testcase();
    void case5_testcase();
    void case6_testcase();
    void case7_testcase();
    void case8_testcase();
    void case9_testcase();
    void case10_testcase();
    void case11_testcase();
    void case12_testcase();
    void case13_testcase();
    void case14_testcase();
    void case15_testcase();
    void case16_testcase();
    void case17_testcase();

public:
    explicit LogicTest(QObject *parent = nullptr);

signals:

};

#endif // LOGICTEST_H
