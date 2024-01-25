#include "Class.h"
#include <string>
#include "Student.h"
#include "iostream"
using namespace std;

void Class::addStudent(const Student &st)
{
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper &Class::getStudentWrapper(const std::string &studentId) {

        for (std::vector<StudentWrapper>::iterator it = students.begin();
             it != students.end();
             ++it) {
            if (it->id == studentId)
                return *it;
        }

        throw "No matching student!";



//std::cout<<"No match student!"<<std::endl;
    /*catch(const char* e)
    {
        cout<<"No match student!"<<endl;
    }*/
}



double Class::getAvgScore()
{
    // TODO: implement getAvgScore.
    double allScore;
    int num=0;
    for(std::vector<StudentWrapper>::iterator it = students.begin();
        it != students.end();
        ++it) {
        num++;
        allScore+=it->getScore();
    }
    double averageScore=allScore/num;

    return averageScore;
}
