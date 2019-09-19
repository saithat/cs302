// include header file(s) needed
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

#include "Slist.h"

// modify the data class and the associated input 
// and output operators to store, set and print a 
// line number identifier called ID

class data { 
   public:
	// add operator< using lastname, firstname, phone number
	
	friend bool operator<(const data &,const data &);
  

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

  private:
    string firstname;
    string lastname;
    string phonenum;
    int ID;
 };
istream & operator>> (istream &in, data &r) {
  in >> r.firstname >> r.lastname >> r.phonenum >> r.ID;
 }
ostream & operator<< (ostream &out, const data &r) { 
  string name = r.lastname + " " + r.firstname;
  out << setfill(' ');
  out << " " << setw(24) << left << name;
  out << setw(12) << r.phonenum;
  out << setw(9) << right << r.ID << endl;
 }

 bool operator<(const data &l,const data &r) 
{
	if(l.lastname.compare(r.lastname) < 0)
	{
		return true;
	} else if (l.lastname.compare(r.lastname) > 0)
	{
		return false;
	} else if (l.firstname.compare(r.firstname) < 0)
	{
		return true;
	} else if (l.firstname.compare(r.firstname) > 0)
	{
		return false;
	}else if (l.phonenum.compare(r.phonenum) < 0)
	{
		return true;
	}
	return false;
}
template <typename T>
void printlist(const T &begin, const T &end) { 
  for(typename T::iterator it = begin; it != end; ++it)
  {
	  cout << *it;
  }
}

int main(int argc, char *argv[]) {
  // copy command-line check from QsortB.cpp,
  // then modify to apply to this assignment
  string usage = "usage: ./Slist_usage -quicksort|mergesort < file.txt";
	if (argc != 2){
		cerr << usage << endl;
		return 1;
	}

	bool quicksort = false;
	bool mergesort = false;	
	if(string(argv[0]) != "./Slist_usage"){
		cerr << usage << endl;
		return 1;
	} 
	if (string(argv[1]) == "-quicksort"){
		quicksort = true;
	} 
	if (string(argv[1]) == "-mergesort"){
		mergesort = true;
	}
  int id = 1;
  slist<data> A;

  data din;
  string line;
  while (getline(cin, line))  //using stringstreams to insert ID
  {
	stringstream num;
	num << id;
	string str2 = num.str();

	line  = line + " " + str2;
	num.str("");
	num.clear();
    id++;
	stringstream ss(line);
	ss >> din;
    A.push_back(din);
  }

  if(quicksort)
  {
    A.sort("quicksort");
  }
    else
    {
      A.sort("mergesort");
    }
    

  printlist(A.begin(), A.end());
}
