#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <iomanip>
using namespace std;

void set_oldfriends(vector <string> &name, vector< vector<int> > &friends, int M0, int M1) {
  int N = (int)name.size();
  //initialize 2D-vector-array friends (hint: use vector::assign())
  //vector<vector<int>> friends = friends;
  friends.assign(N, vector<int>(N,0));
  int i, j;
  set<int>::iterator ii;
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
      friends[i][j] = 1;
      friends[j][i] = 1;
	//use iterator to sweep thru doknow set
	//update the corresponding pairs of friends entries
    }
  }
}

void set_newfriends(vector<vector<int> > &friends, vector<vector<int> > &new_friends) {
  int N = (int)friends.size();
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.assign(N, vector<int>(N,0));
  for (int i=0; i<N; i++) {
    for(int j = 0; j < N; j++) {
      if((i != j) && (friends[i][j]))
      {
        for(int k = 0; k < N; k++)
        {
          if((k!=i) && friends[j][k] && !friends[i][k])
          {
            new_friends[i][k] = 1;
            new_friends[k][i] = 1;
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

void writetofile(const char *fname, vector<string> &name, vector<vector<int> > &friends) {
  //open file
  ofstream fout;
  fout.open(fname);
  int N = (int)name.size();
  int i, j;
  //determine max name length
  for (i=0; i<N; i++) {
	//for (each adjacent friend: friends[i][j] == 1) {
	  //pretty-print name[i] and name[j] (see assignment)
    int count = 0;
    fout << setw(15) << left << name[i] << " :";
    for(j=0; j < N; j++)
    {
      if(friends[i][j] == 1)
      {
        if(count == 8)
      {
        fout << endl;
        fout << setw(15) << left << name[i] << " :";
        count = 0;
      }
        fout << " " << setw(15) << left << name[j];
        count++;
      }
    }
    fout << endl;
  }
  fout.close();
  //close file
}

int main(int argc, char *argv[]) {
  //parse argc, argv arguments
  //print usage message and exit if invalid

string usage = "usage: cat datafile.txt | ./Friendnet1 [-seed N]";

	if (argc < 1 || argc > 3){
		cerr << usage << endl;
		return 1;
	}


	int seed = time(NULL);
	bool seed_given = false;	
	if(string(argv[0]) != "./Friendnet1"){
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
  vector<vector<int> > friends;
  //declare 2D-vector-array called new_friends
  vector<vector<int> > new_friends;

  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow1.txt", name, friends);

  set_newfriends(friends, new_friends);
  writetofile("mightknow1.txt", name, new_friends);
}
