#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.

    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();

    return "";
}

// TODO: implement functions which are declared in Student.h.

void Student::addClass(Class *c) {
    bool flag;
    Class *target;
    /* for(std::vector<Class*>::iterator it=classes.begin();it!=classes.end();++it)
     {
         flag=true;
         if((*it)->name==(*c).name)
         {
             flag=false;
             target=*it;
         }
         if(flag==true)
             throw "No matching class!";*/
    classes.push_back(target);

    /* }*/
}
double Undergraduate::getGrade()  {
    {

            double allScore = 0;
            int allPoint = 0;
            for (std::vector<Class *>::iterator it = classes.begin(); it != classes.end(); ++it) {
                double score = (*it)->getStudentWrapper(id).getScore() / 20;
                allPoint += (*it)->point;
                allScore += (*it)->point * score;
            }
            double Grade = allScore / allPoint;
            return Grade;



    }
}
double Graduate::getGrade() {
    int allPoint=0;
    double allScore=0;
    double score;

    for(std::vector<Class *>::iterator it=classes.begin();it!=classes.end();++it)
    {
        double scoretmp=(*it)->getStudentWrapper(id).getScore();
        if(scoretmp<=100&&scoretmp>=90)
            score=4.0;
        if(scoretmp<=89&&scoretmp>=80)
            score=3.5;
        if(scoretmp<=79&&scoretmp>=70)
            score=3.0;
        if(scoretmp<=60&&scoretmp>=60)
            score=2.5;
        if(scoretmp<=60)
            score=2.0;
        allPoint+=(*it)->point;
        allScore+=(*it)->point*score;
    }
    double Grade=allScore/allPoint;
    return Grade;

}