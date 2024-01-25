#include <iostream>
#include <sstream>
#include "Editor.h"
#include "string"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
}
Editor::~Editor()
{
    // TODO: Implement destructor
    delete buffer;
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}
void Editor::cmdAppend()
{
    //it's function:
    //Appends text to the buffer after the current address line. If the current address is zero,
    //the entered text is placed at the beginning of the buffer. Text is entered in input mode.
    //The current address is set to the address of the last line entered or, if there were none,
    //the current address is not changed.
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.

    while(true)
    {
        string text;
        getline(cin,text);
        if(text==".")
            break;
        if (currentLine==0) {
            buffer->insertLine(text,currentLine+1);
            currentLine++;
        }
        else {
            buffer->insertLine(text,currentLine+1);
            currentLine++;
        }

    }

}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true)
    {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        /*if (firstLine) {
            buffer->insertLine(text,currentLine);
            firstLine = false;
        }  else {
            buffer->insertLine(text,currentLine+1);
            currentLine++;
        }*/
        if(currentLine==0)
        {
            firstLine= false;

            buffer->insertLine(text,currentLine+1);
            currentLine++;
            currentLine++;
        }
        else
        {
            firstLine= false;
            buffer->insertLine(text,currentLine);
            currentLine++;
        }
    }
    if(firstLine== false)
    {
        currentLine--;
    }

}

void Editor::cmdDelete(int start, int end)
{
    if(end<start)
        throw "Delete range error";
    if(start<=0||end>buffer->list->currentLength)
        throw "Line number out of range";
    bool  flag= true;
    bool anot= true;
    if(end==buffer->list->currentLength)
        flag=false;

    buffer->deleteLines(start, end);
    if(flag== false)
        currentLine=start-1;



        else
        {
            currentLine=start;
        }



    //cout<<currentLine<<endl;
}

void Editor::cmdNull(int line)
{
    if(line<=0||line>buffer->list->currentLength)
        throw "Line number out of range";
    cout << buffer->moveToLine(line) << endl;
    currentLine=line;
}

void Editor::cmdNumber(int start, int end)
{
    if(end<start)
        throw "Number range error";
    if(start<=0||end>buffer->list->currentLength)
        throw "Line number out of range";
    buffer->showLines(start, end);
    currentLine=end;
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);

}

void Editor::dispatchCmd(const string &cmd)
{
    /*if(cmd=="r")
    {
        cout<<returnCurrentLine()<<endl;
        return;
    }*/
    if (cmd == "a") {
        cmdAppend();
        return;
    }

    if (cmd[0] == 'w' && cmd[1] == ' ') {
        // TODO: call cmdWrite with proper arguments
        string a;
        a=cmd.substr(2);
        if(a=="")
            throw "Filename not specified";
        cmdWrite(a);
        return;
    }
    // TODO: handle special case "1,$n".
    if(cmd=="1,$n")
    {
        if(buffer->list->currentLength==0)
            throw "Number range error";
        cmdAll();
        currentLine=buffer->list->currentLength;
        //cout<<currentLine<<endl;
        return;
    }

    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        if (type == 'n') {
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            cmdDelete(start, end);
            return;
        }
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    throw "Bad/Unknown command";
}

void Editor::cmdAll() {
    buffer->list->traverse();
}
