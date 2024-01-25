#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>
#include "iostream"
using namespace std;

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:

protected:




public:
    const std::string id;
    const std::string name;
    const std::string year;
    Degree degree;
    std::vector<Class*> classes;
    std::string toString() const;
    virtual double getGrade()=0;
    void addClass(Class* c);
    Student(std::string a,std::string b,std::string c):id(a),name(b),year(c)
    {

    }
};

// TODO: implement class Graduate.
class Graduate:public Student{
public:
   /* Degree degree=graduate;*/
    Graduate(std::string a,std::string b,std::string c): Student(a,b,c)
    {
        degree=graduate;
    }
    virtual double getGrade();


};

// TODO: implement class Undergraduate.
class Undergraduate:public Student{
public:
    /*Degree degree=undergraduate;*/

    Undergraduate(std::string a,std::string b,std::string c): Student(a,b,c)
    {
        degree=undergraduate;
    }
    virtual double getGrade();

};

class StudentWrapper {
private:

    double score = 0.0;
public:
    const Student &student;
    const std::string id;
    // TODO: fix error
    //solved
    StudentWrapper(const std::string &id, const Student &student) :student(student),id(id)
    {
    }

    void setScore(double score)
    {

           if (score < 0 || score > 100)
           {
               throw "Wrong score!";
               //std::cout<<"Wrong score!"<<std::endl;
           }
           this->score = score;

     /*  catch (const char* e)
       {cout<<"Wrong score!"<<endl;}*/
    }


    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
