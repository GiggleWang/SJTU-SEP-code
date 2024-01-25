#pragma once
#include "queue.h"
#include "stack.h"
class Board {
    // TODO
    Stack<int>* firstOne;
    Stack<int>* secondOne;
    Stack<int>* thirdOne;
    int sizeOfBoard;
    Stack<int>* input;
public:
    Board(int num_disk);
    ~Board();
    void draw();
    void move(int from, int to, bool log = true);
    bool win();
    void autoplay();
    void autoHanoi(int size,int a,int b,int c);
};
