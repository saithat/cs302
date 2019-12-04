//#include  WHAT'S NEEDED
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

#include "Person.h"

//PERSON CLASS MEMBER FUNCTION IMPLEMENTATION
ostream& operator<<(ostream& out, person& p)
{
    p.print_personinfo();
    p.print_courseinfo();
}

bool person::operator<(person& rhs)
{
    if(type < rhs.type)
    {
        return true;
    }
    if(type == rhs.type)
    {
        if(name.compare(rhs.name) < 0)
        {
            return true;
        }
    }
    return false;
}

//FACULTY CLASS MEMBER FUNCTION IMPLEMENTATION
bool faculty::operator<(faculty& rhs)
{
    if(category < rhs.category)
    {
        return true;
    }
    if(category == rhs.category)
    {
    if(name.compare(rhs.name) < 0)
    {
        return true;
    }
    }
    return false;
}
void faculty::print_personinfo()
{
    cout << setw(12) << right << "Name: " << "Dr. " << name << endl;
    string cat;
    switch (category)
    {
        case 1:
            cat = "Assistant Professor";
            break;
        case 2:
            cat = "Associate Professor";
            break;
        case 3:
            cat = "Full Professor";
            break;
        default:
            cat = "Faculty";
    }
    cout << setw(12) << right << "Category: " << cat << endl;
}
void faculty::print_courseinfo()
{
    sort(courselist.begin(), courselist.end());
    for(vector<string>::iterator it = courselist.begin(); it != courselist.end(); it++)
    {
        cout << setw(12) << right << "Course: " << *it << endl;
    }
    cout << endl;
}
void faculty::add_course(const string& course)
{
    courselist.push_back(course);
}
//STUDENT CLASS MEMBER FUNCTION IMPLEMENTATION
bool student::operator<(student& rhs)
{
    
    if(category < rhs.category)
    {
        return true;
    }
    if(category == rhs.category)
    {
    if(name.compare(rhs.name) < 0)
    {
        return true;
    }
    }
    return false;
}
void student::print_personinfo()
{
    cout << setw(12) << right << "Name: " << name << endl;
    string cat = "";
    switch (category)
    {
        case 11:
            cat = "Freshman";
            break;
        case 12:
            cat = "Sophomore";
            break;
        case 13:
            cat = "Junior";
            break;
        case 14:
            cat = "Senior";
            break;
        default:
            cat = "Student";
    }
    cout << setw(12) << right << "Category: " << cat << endl;
}
void student::print_courseinfo()
{
    sort(courselist.begin(), courselist.end());
    float sum;
    int count = 0;
    for(vector<pair<string,float> >::iterator it = courselist.begin(); it != courselist.end(); it++)
    {
        count++;
        sum += (*it).second;
        float average = sum/count;
        cout << setw(12) << right << "Course: " << setw(26) << left << (*it).first << fixed << setprecision(2) << setw(3) << (float)(*it).second << " " << average << endl;
    }
    cout << endl;
}
void student::add_course(const pair<string, float> &bar)
{
    courselist.push_back(bar);
}