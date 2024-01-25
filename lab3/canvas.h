#pragma once

#include <iostream>

using namespace std;

class Canvas {
public:
    static const int HEIGHT = 11, WIDTH = 41;
    char buffer[HEIGHT][WIDTH];
    
    void draw() const {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++)
                cout << buffer[i][j];
            cout << endl;
        }
    }
    void set(Stack<int>* a,Stack<int>* b,Stack<int>* c)
    {
        //cout<<a->size()<<" "<<b->size()<<" "<<c->size()<<endl;
        setEmpty();
        setStar(5,a);
        setStar(20,b);
        setStar(35,c);

    }
    void setEmpty()
    {
        for(int j=0;j<WIDTH;j++)
        {
            buffer[HEIGHT-1][j]='-';
        }
        for(int i=0;i<HEIGHT;i++)
        {
            buffer[i][5]='|';
        }
        for(int i=0;i<HEIGHT;i++)
        {
            buffer[i][20]='|';
        }
        for(int i=0;i<HEIGHT;i++)
        {
            buffer[i][35]='|';
        }
    }
    void setStar(int numOfLine,Stack<int>* aimStack)
    {
        Stack<int> *tmp1;
        Stack<int> *tmp2;
        tmp1=new Stack<int>(aimStack->size());
        tmp2=new Stack<int>(aimStack->size());
        int a=aimStack->size();
        for(int i=0;i<a;i++)
        {
            int tmp=aimStack->top();
            aimStack->pop();
            tmp1->push(tmp);
            tmp2->push(tmp);
        }
        for(int i=0;i<a;i++)
        {
            int tmp=tmp1->top();
            tmp1->pop();
            aimStack->push(tmp);
        }

        for(int i=1;i<=a;i++)
        {
            int tmp=tmp2->top();
            tmp2->pop();
            for(int j=-tmp;j<=tmp;j++)
            {
                buffer[11-2*i][numOfLine+j]='*';
            }
        }
    }
    void reset() {
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
                buffer[i][j] = ' ';
    }
};