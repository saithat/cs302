//WRITE THIS BASED ON SMART POINTER CLASS
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

template <typename T>
class sptr {
  public:
    sptr(T *_ptr=NULL) { ptr = _ptr; }
    bool operator< (const sptr &rhs) const {
      return *ptr < *rhs.ptr;
    }
    operator T * () const { return ptr; }
  private:
    T *ptr;
};

template <typename T>
void sptrsort(std::vector<T *> &A) {
  //USE std::sort on SPTR VERSION OF A
  //NOTE: A is vector of pointers (T *)
  
  // set up smart pointer array called Ap
  vector<sptr<T> > Ap;
  for(typename vector<T*>::iterator it = A.begin(); it != A.end(); it++)
  {
    Ap.push_back(*it);
  }
  sort(Ap.begin(), Ap.end());

  for(int i = 0; i < A.size(); i++)
  {
    A[i] = Ap[i];
  }

}


