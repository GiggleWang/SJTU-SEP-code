#include <vector>
#include <iostream>
#include <fstream>
#include "Class.h"
#include "Student.h"
#include "iomanip"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printAvgScore();
    void printGpa();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("./Students.txt");

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each columnes in the line.
        bufv.clear();
        // Split the line into columes.
        //   pos1: begining the the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.

        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);


        studentVec.push_back(st);
    }
    stfile.close();

    // TODO: load data from ./Classes.txt and push objects to the vector.
    ifstream afile("./Classes.txt");

    while(getline(afile,line)){
        if(line.empty())
            continue;
        if(line[0]=='#')
            continue;
        //bufv.clear();
        if(line[0]=='C'&&line[1]=='l'&&line[2]=='a'&&line[3]=='s')
        {
            clsname=line.substr(6);
        }
        if(line.substr(0,6)=="Points")
        {
            point=line[7]-'0';
            cl=new Class(clsname,point);
            //此处还应该录入学生选课情况。
            while(getline(afile,line))
            {
                if(line.empty())
                    break;
                else
                {
                    string tmp=line.substr(0);

                    for(std::vector<Student *>::iterator it=studentVec.begin();it!=studentVec.end();it++)
                    {
//                        cout<<(*it)->id<<endl;


                        if(tmp==(*it)->id){
                            const Student* tmpOfStudent=*it;
                            cl->addStudent(*tmpOfStudent);
                            (*it)->addClass(cl);
                        }
                    }


                }
            }
            classVec.push_back(cl);
        }


    }
    afile.close();


    /*ifstream bfile("../Classes.txt");
    while(getline(bfile,line)){
        Class* tmpClass;
        if(line.empty())
            continue;
        if(line[0]=='#')
            continue;
        if(line[0]=='C'&&line[1]=='l'&&line[2]=='a'&&line[3]=='s')
        {
            clsname=line.substr(6);
            for(std::vector<Class*>::iterator it=classVec.begin();it!=classVec.end();it++)
            {
                if(clsname==(*it)->name)
                    tmpClass=*it;
            }

            while (getline(bfile,line))
            {
                if(line.empty())
                    break;
                if(line[0]=='P'&&line[1]=='o')
                    break;
                else
                {
                    string tmpName=line.substr(0);
                    for(std::vector<Student *>::iterator it=studentVec.begin();it!=studentVec.end();++it)
                    {
                        if(tmpName==(*it)->name)
                            (*it)->addClass(tmpClass);
                    }
                }
            }
        }



    }
    bfile.close();*/

    // Hint: how is student information read?
}

void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printAvgScore().



    while(true) {
        string sbuf;
        Class *cl;

        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
             it != classVec.end();
             ++it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cout << "No match class!" << endl;
            continue;
        }

        while (true) {
            string inputString;
            //StudentWrapper* temp;
            bool flag = true;

            cout << "Please input the student number (or input q to quit): ";
            cin >> inputString;
            if (inputString == "q")
                break;

            for (vector<StudentWrapper>::iterator it = cl->students.begin(); it != cl->students.end(); it++) {
                if (it->id == inputString) {
                    cout<<it->toString();
                    cout << "please input the score: ";
                    double inputScore;
                    string stringA;
                    cin>>stringA;
                    if(stringA=="q")
                        break;
                    bool flag1=true;
                    if(stringA[1]=='\0'||stringA[2]=='\0'||stringA[3]=='\0')
                        flag1=false;
                    if(flag1== true)
                    {
                        cout<<"Wrong score!"<<endl;
                        flag=false;
                        continue;

                    }
                    if(stringA[1]=='\0')
                    {
                        if(stringA[0]-'0'>=0&&stringA[0]-'0'<=9)
                            inputScore=stringA[0]-'0';
                        else
                        {
                            cout<<"Wrong score!"<<endl;
                            flag=false;
                            continue;

                        }
                    }

                    if(stringA[2]=='\0'&&stringA[1]!='\0')
                    {
                        if((stringA[0]-'0'>=0&&stringA[0]-'0'<=9)&&(stringA[1]-'0'>=0&&stringA[1]-'0'<=9))
                        {
                            inputScore=(stringA[0]-'0')*10+stringA[1]-'0';
                        }
                        else
                        {
                            cout<<"Wrong score!"<<endl;
                            flag=false;
                            continue;

                        }
                    }

                    if(stringA[3]=='\0'&&stringA[2]!='\0')
                    {
                        if(stringA=="100")
                            inputScore=100;
                        else
                        {
                            cout<<"Wrong score!"<<endl;
                            flag=false;
                            continue;

                        }
                    }
                    if(inputScore<0||inputScore>100)
                    {
                        cout<<"Wrong score!"<<endl;
                        flag=false;
                        continue;

                    }


                    try{it->setScore(inputScore);}
                    catch(char const *s2){
                        cout<<"Wrong score!"<<endl;
                    }
                    flag = false;

                    continue;
                }
            }

            if (flag == true) {
                cout << "No match student!" << endl;
                continue;
            }


        }
    }
}

void AppX::printAvgScore()
{
    string sbuf;
    Class *cl;
    double avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
             it != classVec.end();
             ++it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cout << "No match class!" << endl;
            continue;
        }

        avg = cl->getAvgScore();
        cout << "The average score is: " ;
        cout<<setiosflags(ios::fixed)<<setprecision(2)<< avg << endl;
    }
}

void AppX::printGpa()
{
    // TODO: implement printGpa.
    // Hint: Take a look at printAvgScore().
    while(true)
    {
        string inputString;
        cout<<"Please input the student number (or input q to quit): ";
        cin>>inputString;
        if(inputString=="q")
            break;

        Student* cl;
        cl= nullptr;
        for(vector<Student *>::iterator it =studentVec.begin();it!=studentVec.end();++it)
        {
            if((*it)->id==inputString)
            {
                cl=*it;
                break;
            }

        }
        if(cl== nullptr)
        {
            cout<<"No match student!"<<endl;
            continue;
        }
        try{double Grade=cl->getGrade();
            cout<<"GPA = ";
            cout<<setiosflags(ios::fixed)<<setprecision(2)<<Grade<<endl;}
        catch(char const *s1){
            cout<<"No match student!"<<endl;
        }

    }
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute average score of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printAvgScore();
        } else if (cmd == 'g') {
            printGpa();
        } else if (cmd == 'q') {
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
