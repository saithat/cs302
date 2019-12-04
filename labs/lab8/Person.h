#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
                 STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

class person 
{
	friend ostream& operator<<(ostream&, person&);
    public:
        person() {;}
		virtual ~person() {;}
		virtual bool operator<(person&);
		//virtual void add_course(const string&);
		//virtual void add_course_student(pair<string,float>&);
		void set_name(const string& n_name)
		{
			name = n_name;
		}
		void set_type(type_enum typ)
		{
			type = typ;
		}
		void set_category(type_enum cat)
		{
			category = cat;
		}
		string get_name() {return name;}
		type_enum get_type(){return type;}
		type_enum get_category() {return category;}

	protected:
		string name;
		type_enum type;
		type_enum category;
		virtual void print_personinfo() = 0;
		virtual void print_courseinfo() = 0;

};
//DEFINITION -- base class

//class faculty DEFINITION -- derived class
class faculty:public person
{
	vector<string> courselist;
	void print_personinfo();
	void print_courseinfo();
	public:
		faculty() {;}
		~faculty() {;}
		bool operator<(faculty&);
		void add_course(const string&);
};

//class student DEFINITION -- derived class
class student:public person
{
	vector<pair<string, float> > courselist;
	void print_personinfo();
	void print_courseinfo();
	public:
		student() {;}
		~student() {;}
		bool operator<(student&);
		void add_course(const pair<string, float> &);
		float gp;
};

#endif
