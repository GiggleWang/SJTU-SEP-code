#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//first we can implement the functions in list;
edlist::edlist()
{
    head=new node;
    currentLength=0;
}

void edlist::clear() {
    node*p=head->next,*q;
    head->next=NULL;
    while(p!=NULL)
    {
        q=p->next;
        delete p;
        p=q;
    }
    currentLength=0;
}

edlist::node *edlist::move(int i) const {
    node*p=head;
    while(i-->0)
    {
        p=p->next;
    }
    return p;
}

void edlist::insert(int i, const std::string &x) {
    node* pos;
    pos=move(i-1);
    pos->next=new node(x,pos->next);
    ++currentLength;
}

void edlist::remove(int i) {
    node *pos,*delp;
    pos=move(i-1);
    delp=pos->next;
    pos->next=delp->next;
    delete delp;
    --currentLength;
}

int edlist::search(const std::string &x) {
    node*p=head->next;
    int i=0;
    while(p!=NULL&&p->data!=x)
    {
        p=p->next;
        i++;
    }
    if(p==NULL)
        return -1;
    else return i;
}

void edlist::traverse() const {
    node*p=head->next;
    int i=0;
    while(p!=NULL)
    {
        i++;
        std::cout<<i<<'\t';
        std::cout<<p->data<<std::endl;
        p=p->next;
    }
}

int edlist::length() const {
    return currentLength;
}

string edlist::visit(int i) {
    node* tmp= move(i);
    std::cout<<tmp->data;
    std::cout<<std::endl;
}

//implement the functions in ListBuffer
Buffer::Buffer() {
    list=new edlist;
}

Buffer::~Buffer() {
    delete list;
}

void Buffer::writeToFile(const string &filename) const
{
    int allByte=0;
    std::ofstream afile(filename);
    edlist::node *p=list->head->next;
    while(p!=NULL)
    {
        allByte+=p->data.size();
        afile<<p->data;
        p=p->next;
        allByte++;
    }
    std::cout<<allByte<<" byte(s) written"<<std::endl;
    afile.close();
}

void Buffer::showLines(int from, int to) const
{
    for(int i=from;i<=to;i++)
    {
        edlist::node* tmp=list->move(i);
        std::cout<<i<<'\t';
        std::cout<<tmp->data<<std::endl;
    }
}

void Buffer::deleteLines(int from, int to)
{
    for(int i=0;i<=to-from;i++)
    {
        list->remove(from);
    }

}

void Buffer::insertLine(const string &text,int a)
{
    list->insert(a,text);
    currentLineNum=a;
}

void Buffer::appendLine(const string &text)
{
    insertLine(text,currentLineNum);
}

const string &Buffer::moveToLine(int idx) const
{
    edlist::node* tmp=list->move(idx);
    return tmp->data;

}
