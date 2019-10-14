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
  friends.assign(N, vector<int>());
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
      friends[i].push_back(j);
      friends[j].push_back(i);
	//use iterator to sweep thru doknow set
	//update the corresponding pairs of friends entries
    }
  }
  //cout << "end of old friends" << endl;
}

void set_newfriends(vector<vector<int> > &friends, vector<vector<int> > &new_friends) {
  int N = (int)friends.size();
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.assign(N, vector<int>());
   vector<int>::iterator members;
   vector<int>::iterator kmembers;
  //cout << "inside newfriends" << endl;
  for (int i=0; i<N; i++) {
    for(members = friends[i].begin(); members != friends[i].end(); members++) {
      if(*members != i)
      {
        for(kmembers = friends[*members].begin(); kmembers != friends[*members].end(); kmembers++)
        {
          if((*kmembers != i) && (find(friends[i].begin(), friends[i].end(), *kmembers) == friends[i].end()))
          {
            new_friends[i].push_back(*kmembers);
            new_friends[*kmembers].push_back(i);
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
  int i;
  vector<int>::iterator members;
  vector<int>::iterator it;
  //determine max name length
  for (i=0; i<N; i++) {
	//for (each adjacent friend: friends[i][j] == 1) {
	  //pretty-print name[i] and name[j] (see assignment)
    sort(friends[i].begin(), friends[i].end());
    it = unique(friends[i].begin(), friends[i].end());
    friends[i].resize( distance(friends[i].begin(),it) );
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

string usage = "usage: cat datafile.txt | ./Friendnet2 [-seed N]";

	if (argc < 1 || argc > 3){
		cerr << usage << endl;
		return 1;
	}


	int seed = time(NULL);
	bool seed_given = false;	
	if(string(argv[0]) != "./Friendnet2"){
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
  writetofile("doknow2.txt", name, friends);

  set_newfriends(friends, new_friends);
  writetofile("mightknow2.txt", name, new_friends);
}
