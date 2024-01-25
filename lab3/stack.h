//finished
#pragma once

#include <cassert>

template <typename elemType> class Stack {
    // TODO
private:
    elemType* elem;
    int top_p;
    int maxSize;
    void doubleSpace(){
        elemType* tmp=elem;
        elem=new elemType[2*maxSize];
        for(int i=0;i<maxSize;i++)
        {
            elem[i]=tmp[i];
        }
        maxSize*=2;
        delete[] tmp;

    }
 public:
    Stack(int initSize) {
        // TODO
        elem=new elemType[initSize];
        maxSize=initSize;
        top_p=-1;
    }
    
    ~Stack() {
        // TODO
        delete[] elem;
    }
    
    int size() {
        int s=top_p+1;
        return s;
    }
    
    bool isEmpty() const {
        return size() == 0;
    }
    
    void push(const elemType &t) {
        // TODO
        if(top_p==maxSize-1)
            doubleSpace();
        elem[++top_p]=t;
    }
    
    void pop() {
        // TODO
        top_p--;
    }
    
    elemType& top() const {
        // TODO
        return elem[top_p];
    }
};
