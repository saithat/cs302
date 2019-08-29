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

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};

istream & operator>>(istream &in, data &r) { 
  // write this to read data object data

}

ostream & operator<<(ostream &out, const data &r) {
  // write this to write data object data
}

void printlist(...) { 
  // template based iterator code for printing data to stdout
}

int main(int argc, char *argv[]) {
  vector<data> A;

  data din;
  while (cin >> din)
    A.push_back(din);
  
  std::sort(A.begin(), A.end();

  printlist(A.begin(), A.end());
}
