#include "board.h"
#include "canvas.h"

Board::Board(int num_disk) {
    // TODO
    sizeOfBoard=num_disk;
    firstOne=new Stack<int>(num_disk+1);
    secondOne=new Stack<int>(num_disk+1);
    thirdOne=new Stack<int>(num_disk+1);
    input=new Stack<int>(10000);
    for(int i=num_disk;i>0;i--)
    {
        firstOne->push(i);
    }
}
Board::~Board() {
    // TODO
    delete firstOne;
    delete secondOne;
    delete thirdOne;
    delete input;
}

void Board::draw() {
    Canvas canvas;
    canvas.reset();
    // TODO
    canvas.set(firstOne,secondOne,thirdOne);
    canvas.draw();
}

void Board::move(int from, int to, bool log) {
    // TODO
    if(log==false)
        cout<<"Auto moving:"<<from<<"->"<<to<<endl;
    Stack<int>*tmp1;
    Stack<int>*tmp2;
    switch (from){
        case 1:tmp1=firstOne;
            break;
        case 2:tmp1=secondOne;
            break;
        case 3:tmp1=thirdOne;
            break;
    }
    switch (to){
        case 1:tmp2=firstOne;
            break;
        case 2:tmp2=secondOne;
            break;
        case 3:tmp2=thirdOne;
            break;
    }
    if(tmp1->size()!=0)
    {
        if(tmp1->top()<tmp2->top()||tmp2->size()==0)
        {
            int mid;
            mid=tmp1->top();
            tmp1->pop();
            tmp2->push(mid);
            if(log==true)
            {
                input->push(from);
                input->push(to);
            }
        }
    }


    draw();

}

bool Board::win() {
    // TODO
    if(secondOne->size()!=sizeOfBoard)
        return false;
    Stack<int>* tmpOne;
    tmpOne=new Stack<int>(sizeOfBoard);
    bool flag=true;
    for(int i=1;i<=sizeOfBoard;i++)
    {
        int tmp=secondOne->top();
        secondOne->pop();
        if(tmp!=i)
            flag=false;
        tmpOne->push(tmp);
    }
    for(int i=1;i<=sizeOfBoard;i++)
    {
        int tmp=tmpOne->top();
        tmpOne->pop();
        secondOne->push(tmp);
    }
    return flag;
}

void Board::autoplay() {
    // TODO
    //firstly,return to the original condition
    int a=input->size();
    for(int i=0;i<a/2;i++)
    {
        int from,to;
        from=input->top();
        input->pop();
        to=input->top();
        input->pop();
        move(from,to,false);
    }
    //secondly,do as the auto moves;
    autoHanoi(sizeOfBoard,1,2,3);
}
void Board::autoHanoi(int size,int a,int b,int c) {
    if(size==1)
    {
        move(a,b,false);
    }
    else{
        autoHanoi(size-1,a,c,b);
        move(a,b, false);
        autoHanoi(size-1,c,b,a);
    }

}