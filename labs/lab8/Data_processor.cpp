#include <iostream>
#include <string>
#include <stdlib.h> //std::stof from string is not recognized
using namespace std;

#include "Person.h"
#include "Sptrsort.h"

type_enum str2type(const string& s)
{
	if(s.compare("Assistant Professor") == 0)
	{
		return ASST_PROF;
	}
	else if(s.compare("Associate Professor")==0)
	{
		return ASSOC_PROF;
	}
	else if(s.compare("Full Professor")==0)
	{
		return FULL_PROF;
	}
	else if(s.compare("Freshman")==0)
	{
		return FRESHMAN;
	}
	else if(s.compare("Sophomore")==0)
	{
		return SOPHOMORE;
	}
    else if(s.compare("Junior")==0)
	{
		return JUNIOR;
	}
    else if(s.compare("Senior")==0)
	{
		return SENIOR;
	}
}

int main(int argc, char *argv[]) {
	string error_msg = "usage: ./Data_processor -person|faculty|student < file.xml";
  if (argc != 2) {
    cerr << error_msg << endl;
	return 1;
  }

  person *n_person;
  faculty *f_person;
  student *s_person;

  vector<person *> person_list;
  //vector<faculty *> faculty_list;
  //vector<student *> student_list;

	bool per = false;
	bool fac = false;
	bool stud = false;
  if(string(argv[1]) == "-person")
  {
	  per = true;
	  //person *n_person;
	  //vector<person *> person_list;
  } else if(string(argv[1]) == "-faculty")
  {
	  fac = true;
	  //faculty *n_person;
	  //vector<faculty *> person_list;
	  //cout << "redeclared shit as faculty" << endl;
  } else if(string(argv[1]) == "-student")
  {
	  stud = true;
	  //student *n_person;
	  //vector<student *> person_list;
  } else
  {
	  cerr << error_msg << endl;
	  return 1;
  }
  

  type_enum type = UNKNOWN;
  type_enum g = UNKNOWN;
  int line = 1;
  size_t iL, iR;
  string input;
  //CS307: CHECK CORRECTESS OF XML HEADER 
	//cout << "about to read in file" << endl;
  while (getline(cin, input)) {
	++line;
	//cout << "inside while loop reading input. line: " << line <<"input: " << input << endl;
	if ((iL = input.find('<')) == string::npos) {
	  continue;
    } else if (input.compare(iL,9,"<faculty>") == 0) {
	  //set type
	  type = FACULTY;
	  n_person = new faculty();
	  f_person = static_cast<faculty*>(n_person);
	  //course.clear();

	} else if (input.compare(iL,10,"</faculty>") == 0) {

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR ADDING FACULTY PERSON TO DATABASE

	  //check type 
	  //set subtype
	
		//dynamic cast n_person to faculty or student

	  //n_person = new faculty();
	  //f_person = static_cast<faculty*>(n_person);
	  //cout << "inside file, about to declare faculty object" << endl;
	  //cout << "setting faculty type" << endl;
	  n_person->set_type(type);
	  person_list.push_back(n_person);
	  //reset type
	  type = UNKNOWN;
	  //reset subtype
	  //g = UNKNOWN;

	  continue;

	} else if (input.compare(iL,9,"<student>") == 0) {
	  //set type
	  type = STUDENT;
	  n_person = new student();
	  s_person = dynamic_cast<student*>(n_person);
	  if(!s_person)
	  {
		  cout << "s_person is NULL" << endl;
	  }
	  //cout << "casting student" << endl;
	  //n_person = new student();
	  //course.clear();
	  //gp.clear();

	} else if (input.compare(iL,10,"</student>") == 0) {

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR ADDING STUDENT PERSON TO DATABASE

	  //check type 
	  //set subtype
		//student *s_person = static_cast<student*>(n_person);
	  //n_person = new student();
	  //cout << "setting student type" << endl;
	  n_person->set_type(type);
	  person_list.push_back(n_person);
	//cout << "set student object, type and pushed to list" << endl;
	  //reset type
	  type = UNKNOWN;
	  //reset subtype
	  g = UNKNOWN;

	  continue;

	} else if (input.compare(iL,5,"<name") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);
		//cout << "inside name and found iL and iR" << endl;
      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR EXTRACTING NAME (using iL, iR)
	  //cout << "name: " << input.substr(iL + 2, iR - 2 - iL) << endl;
	  //cout << n_person->get_name() << endl;
	  n_person->set_name(input.substr(iL + 2, iR - 2 - iL));
		//cout << "set student name" << endl;
		//"<g="
	} else if (input.compare(iL,10,"<category=") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR EXTRACTING CATEGORY (using iL, iR)
	  //
	  n_person->set_category(str2type(input.substr(iL + 2, iR - 2 - iL)));

	} else if (input.compare(iL,7,"<course") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"", iL+2);

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR EXTRACTING COURSE (using iL, iR)
	  if(type == FACULTY)
	  {
		f_person->add_course(input.substr(iL + 2, iR - 2 - iL));
	  }
	  string cour;
	  if(type == STUDENT)
	  {
		  cour = input.substr(iL + 2, iR - 2 - iL);
	  }
	  iL = iR;
	  if (type == FACULTY) {
	    iR = input.find("/>", iL+1);
	  } else if (type == STUDENT) {
	    iL = input.find("gp=\"", iL);
	    iR = input.find("\"/>", iL+4);
		string gp_text = input.substr(iL + 4, iR - 2 - iL);
		//float gp = stof(gp_text, &sz);
		//char *gp_text_c = gp_text.c_str(); 
		//float gp = strtof(gp_text.c_str(), &pEnd);
		//cout << "gp_text: " << gp_text << endl;
		float gp = (float)atof(gp_text.c_str());
		//cout << "gp: " << gp << endl;
		pair<string, float> foo;
		foo = make_pair(cour, gp);
		s_person->add_course(foo);
        //CS307: CHECK CORRECTESS OF XML DATA
	    //CODE FOR EXTRACTING GRADE (using iL, iR)
	  }
	}
  }

  //EXTRACT LIST OF POINTERS TO PERSON, FACULTY OR STUDENT
  //ENTRY, SORT USING SPTRSORT AND PRINT RESULT TO STDOUT
  //vector<sptr<person>> A;
  //cout << "about to smartpointer sort" << endl;

	if(fac)
	{
		vector<faculty *> faculty_list;
		for(vector<person*>::iterator it = person_list.begin(); it!= person_list.end(); it++)
		{
			f_person = dynamic_cast<faculty *>(*it);
			if(f_person)
			{
				faculty_list.push_back(f_person);
			}
		}

		sptrsort(faculty_list);

		for(vector<faculty*>::iterator it = faculty_list.begin(); it!= faculty_list.end(); it++)
		{
			cout << **it;
		}

		return 0;

	}
	if(stud)
	{
		vector<student *> student_list;
		for(vector<person*>::iterator it = person_list.begin(); it!= person_list.end(); it++)
		{
			s_person = dynamic_cast<student *>(*it);
			if(s_person)
			{
				student_list.push_back(s_person);
			}
		}

		sptrsort(student_list);

		for(vector<student*>::iterator it = student_list.begin(); it!= student_list.end(); it++)
		{
			cout << **it;
		}

		return 0;

	}
	sptrsort(person_list);

  //NOTE: PERSON, FACULTY AND STUDENT EACH HAVE THEIR OWN
  //OVERLOADED COMPARISON OPERATOR (--> DIFFERENT SORTING)
	for(vector<person*>::iterator it = person_list.begin(); it!= person_list.end(); it++)
	{
		cout << **it;
	}
  //RELEASE ALL DYNAMICALLY ALLOCATED MEMORY
	while(!person_list.empty())
	{
		delete person_list.back(), person_list.pop_back();
	}

}
