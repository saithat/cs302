#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <iomanip>
using namespace std;

void set_oldfriends(vector <string> &name, vector< set<int> > &friends, int M0, int M1) {
  int N = (int)name.size();
  //initialize 2D-vector-array friends (hint: use vector::assign())
  //vector<vector<int>> friends = friends;
  friends.assign(N, set<int>());
  int i, j;
  set<int>::iterator ii;
  //cout << "inside old friends" << endl;
  for (i=0; i<N; i++) {
    //declare std::set called doknow
    set<int> doknow;
    //initialize M (random number of friends: function of M0, M1)
    int M = M0 + rand() % M1;
    while ((int)doknow.size() < M) {
      //compute j (random friend index: hint j != i)
      do
      {
        j = rand() % N;
      }
      while(i==j);
      doknow.insert(j);
    }
    for(ii = doknow.begin(); ii != doknow.end(); ii++)
    {
      j = *ii;
      friends[i].insert(j);
      friends[j].insert(i);
	//use iterator to sweep thru doknow set
	//update the corresponding pairs of friends entries
    }
  }
  //cout << "end of old friends" << endl;
}

void set_newfriends(vector<set<int> > &friends, vector<set<int> > &new_friends) {
  int N = (int)friends.size();
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.assign(N, set<int>());
   set<int>::iterator members;
   set<int>::iterator kmembers;
  //cout << "inside newfriends" << endl;
  for (int i=0; i<N; i++) {
    for(members = friends[i].begin(); members != friends[i].end(); members++) {
      if(*members != i)
      {
        for(kmembers = friends[*members].begin(); kmembers != friends[*members].end(); kmembers++)
        {
          if((*kmembers != i) && friends[i].find(*kmembers) == friends[i].end())
          {
            new_friends[i].insert(*kmembers);
            new_friends[*kmembers].insert(i);
          }
        }
      }
    }
	/*for (each adjacent friend: friends[i][j] == 1) {
	  for (each adjacent friend-of-friend: friends[j][k] == 1) {
	    if (k-is-not-i && k-is-not-friend-of-i)
	      update the corresponding pairs of new_friends entries
	  }
    }*/
  }
}

void writetofile(const char *fname, vector<string> &name, vector<set<int> > &friends) {
  //open file
  ofstream fout;
  fout.open(fname);
  int N = (int)name.size();
  int i;
  set<int>::iterator members;
  //determine max name length
  for (i=0; i<N; i++) {
	//for (each adjacent friend: friends[i][j] == 1) {
	  //pretty-print name[i] and name[j] (see assignment)
    int count = 0;
    fout << setw(15) << left << name[i] << " :";
    for(members = friends[i].begin(); members != friends[i].end(); members++)
    {
        if(count == 8)
      {
        fout << endl;
        fout << setw(15) << left << name[i] << " :";
        count = 0;
      }
        fout << " " << setw(15) << left << name[*members];
        count++;
    }
    fout << endl;
  }
  fout.close();
  //close file
}

int main(int argc, char *argv[]) {
  //parse argc, argv arguments
  //print usage message and exit if invalid

string usage = "usage: cat datafile.txt | ./Friendnet3 [-seed N]";

	if (argc < 1 || argc > 3){
		cerr << usage << endl;
		return 1;
	}


	int seed = time(NULL);
	bool seed_given = false;	
	if(string(argv[0]) != "./Friendnet3"){
		cerr << usage << endl;
		return 1;
	}
  if(argc > 1)
  { 
    if (string(argv[1]) == "-seed"){
      //cout << "encode is true" << endl;
      seed_given = true;
    } 
    if(seed_given)
    {
      seed = atoi(argv[2]);
    }
  }

  srand(seed);
  //seed random number generator

  vector<string> name;
  //read strings from stdin into name vector
  string line;
  while(getline(cin, line))
  {
    name.push_back(line);
  }

  int M0 = 1; // min number of friends
  int M1 = 3; // potential extra friends

  //declare 2D-vector-array called friends
  vector<set<int> > friends;
  //declare 2D-vector-array called new_friends
  vector<set<int> > new_friends;

  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow3.txt", name, friends);

  set_newfriends(friends, new_friends);
  writetofile("mightknow3.txt", name, new_friends);
}
