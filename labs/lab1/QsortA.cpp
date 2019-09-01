#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
// include header files needed
using namespace std;

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
};

istream & operator>>(istream &in, data &r) { 
  // write this to read data object data
  in >> r.firstname >> r.lastname >> r.phonenum;
}

ostream & operator<<(ostream &out, const data &r) {
  // write this to write data object data
  string name = r.lastname + " " + r.firstname;
  out << setw(24) << left << name  <<  r.phonenum << endl;
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

void printlist(const vector<data>::iterator &begin, const vector<data>::iterator &end) { 
  // template based iterator code for printing data to stdout
  for(vector<data>::iterator it = begin; it != end; it++)
  {
	  cout << *it;
  }
}

int main(int argc, char *argv[]) {
  vector<data> A;

  data din;
  while (cin >> din)
    A.push_back(din);
  
std::sort(A.begin(), A.end());

  printlist(A.begin(), A.end());
}
