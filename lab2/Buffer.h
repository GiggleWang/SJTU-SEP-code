#pragma once

#include <string>

using std::string;
class edlist
{
public:
    void clear();
    int length() const;
    void insert(int i,const string &x);
    void remove(int i);
    int search(const string &x);
    string visit(int i);
    void traverse()const;
    edlist();
    ~edlist(){};
    struct node
    {
        string data;
        node* next;
        node(const string &x,node* n=NULL)
        {
            data=x;next=n;
        }
        node():next(NULL){}
        ~node(){};
    };
    node* head;
    int currentLength;
    node* move(int i)const;
};

class Buffer {
private:
    int currentLineNum;
    // TODO: add a List here for storing the input lines

public:
    Buffer();
    ~Buffer();
    edlist* list;

    void writeToFile(const string &filename) const;

    const string &moveToLine(int idx) const;

    void showLines(int from, int to) const;

    void deleteLines(int from, int to);
    void insertLine(const string &text,int a);
    void appendLine(const string &text);
};
