//finished
#pragma once

#include <cassert>

template <typename elemType> class Queue {
    // TODO
private:
    elemType *elem;
    int maxSize;
    int front,rear;
    void doubleSpace(){
        elemType* tmp=elem;
        elem=new elemType[2*maxSize];
        for(int i=0;i<maxSize;i++)
        {
            elem[i]=tmp[(front+i)%maxSize];
        }
        front=0;rear=maxSize-1;
        maxSize*=2;
        delete tmp;
    }


 public:
    Queue(int size) {
        // TODO
        elem=new elemType[size];
        maxSize=size;
        front=rear=0;
    }
    
    ~Queue() {
        // TODO
        delete[]elem;
    }
    
    int size() const {
        // TODO
        int s=(rear-front+maxSize)%maxSize;
        return s;
    }
    
    bool isEmpty() const {
        return size() == 0;
    }
    
    void enQueue(const elemType& x) {
        // TODO
        if((rear+1)%maxSize==front)
            doubleSpace();
        rear=(rear+1)%maxSize;
        elem[rear]=x;
    }

    elemType deQueue(){
        front =(front+1)%maxSize;
        return elem[front];
    }
    elemType getHead(){
        return elem[front];
    }
    elemType getEnd(){
        return elem[rear];
    }

};
