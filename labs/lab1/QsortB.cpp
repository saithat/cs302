#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
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

istream & operator>>(istream &in, data &r) {		// reading in file data
  in >> r.firstname >> r.lastname >> r.phonenum;
}

ostream & operator<<(ostream &out, const data &r) {  //printing file data
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
  for(vector<data>::iterator it = begin; it != end; it++)
  {
	  cout << *it;
  }
}


template <typename data>
void quicksort(std::vector<data> &A, int left, int right) {
	if(left < right) {
  int pindex = left + rand() % (right-left+1);					//random index selection
  data pivot = A[pindex];
  // partition A: {<=}, {pivot}, {=>} 
  swap(A[pindex], A[right]);
  int i = left - 1;
  int j = right; 
  while (1) {
    while (A[++i] < pivot) {
		if( i == right)
			break;
	}
	while (pivot < A[--j]){
		if (j == left)
			break;
	}
    if (i >= j) 
		break;
    swap(A[i], A[j]);
  }
  pindex = i;
  swap(A[pindex], A[right]);

  quicksort(A, left, pindex-1);
  quicksort(A, pindex+1, right);
}
}

template <typename data>
void quickselect(std::vector<data> &A, int left, int k, int right) {
		if(left < right) {
  int pindex = left + rand() % (right-left+1);
  data pivot = A[pindex];
  // partition A: {<=}, {pivot}, {=>} 
  swap(A[pindex], A[right]);
  int i = left - 1;
  int j = right; 
  while (1) {
    while (A[++i] < pivot) {
		if( i == right)
			break;
	}
	while (pivot < A[--j]){
		if (j == left)
			break;
	}
    if (i >= j) 
		break;
    swap(A[i], A[j]);
  }
  pindex = i;
  swap(A[pindex], A[right]);

		if (pindex == k)
			return;
		if (k < pindex) right = pindex-1;				//recurrence through sublist with k
		else            left = pindex+1;

		quickselect(A, left, k, right);
		}
}

int main(int argc, char *argv[]) {
  // perform command-line error checking
  string usage = "usage: ./QsortB -stl | -rpivot [k0 k1] < file.txt";
	if (argc != 2 && argc != 4){
		cerr << usage << endl;
		return 1;
	}

	bool stl = false;
	bool rpivot = false;
	bool pivots = false;	
	if(string(argv[0]) != "./QsortB"){
		cerr << usage << endl;
		return 1;
	} 
	if (string(argv[1]) == "-stl"){
		stl = true;
	} 
	if (string(argv[1]) == "-rpivot"){
		rpivot = true;
		if ( argc == 4){
			if(atoi(argv[2]) == 0 || atoi(argv[3]) == 0){
				cerr << usage << endl;
				return 1;
			}
			pivots = true;
		}

	}


  vector<data> A;

  data din;
  while (cin >> din)
    A.push_back(din);
  
  // add selection between sorting options

  // option -stl
  if(stl) {
	sort(A.begin(), A.end());
	} else {
  // option -rpivot
  int N = (int)A.size();

  int k0 = 0;
  int k1 = N-1;

  // update k0, k1 if given as command-line arguments,
  if(pivots){
	k0 = atoi(argv[2]);
	if (k0 < 0){
		k0 = 0;
		}
	k1 = atoi(argv[3]);
	if (k1 >= A.size()){
		k1 = A.size()-1;
		}
	}
  // then apply quickselect to partition A accordingly
  // A[0:N-1] --> A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]
  quickselect(A, 0, k0, N-1);
  quickselect(A, k0, k1, N-1);

  quicksort(A, k0, k1);				//sorting in range
  }
  printlist(A.begin(), A.end());
}
