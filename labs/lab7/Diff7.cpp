#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <fstream>
#include <iomanip>

using namespace std;

template <typename T>
class matrix {
  // see handout from class or make your own
  public:
		matrix() {}
		void assign(int n_Nrows=0, int n_Ncols=0){
			Nrows = n_Nrows;
			Ncols = n_Ncols;

			buf = new T *[Nrows];
			for(int i = 0; i<Nrows; i++)	
				buf[i] = new T[Ncols];
		}

		~matrix(){
			for(int i = 0; i <Nrows; i++)
			{
				delete [] buf[i];
			}
			delete [] buf;
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }
		T * operator[] (int i) { return buf[i]; }
	private:
		int Nrows, Ncols;
		T **buf;
};

class LCS {
  public:
    void text1_push_back(string &s)
	{
		text1.push_back(s);
	};
    void text2_push_back(string &s)
	{
		text2.push_back(s);
	};
	//del - vertial, add - horizontal
	void compute_alignment()
	{
		m = text1.size();
		n = text2.size();
		text1.insert(text1.begin(),"-");
		text2.insert(text2.begin(),"-");
		cost.assign(m+1,n+1);
		link.assign(m+1,n+1);

		cost[0][0] = 0;
		link[0][0] = 0;
		for (int i=1; i<=m; i++) {
		cost[i][0] = cost[i-1][0] + DEL(text1[i]);
		//cout << cost[i][0] << endl;
		link[i][0] = VERT;
		}
		for (int j=1; j<=n; j++) {
		cost[0][j] = cost[0][j-1] + INS(text2[j]);
		link[0][j] = HORZ;
		}
		for (int i=1; i<=m; i++) {
			for (int j=1; j<=n; j++) {
				cost[i][j] = cost[i-1][j-1] + SUB(text1[i],text2[j]);
				link[i][j] = DIAG;
				int inscost = cost[i][j-1] + INS(text2[j]);
				if (inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = HORZ;
				}
				int delcost = cost[i-1][j] + DEL(text1[i]);
				if (delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = VERT;
				}
			}
		}
		//cout << "D[m][n] = " << cost[m][n] << "\n";

	};
	void report_difference(int i, int j)
	{
		if((i==0) && (j==0))
		{
			//cout << "base case!" << endl;
			int num_del = 0;
			int num_ins = 0;

			int i = 0, j = 0;
			for(int k = (int)trace.size()-1; 0<=k; k--)
			{
				if(trace[k] == DIAG)
				{
					print_edits(i, j, num_del, num_ins);
					num_del = 0;
					i++;
					num_ins = 0;
					j++;
				}else if(trace[k] == HORZ)
				{
					num_ins++;
					j++;
				}else if(trace[k] == VERT)
				{
					num_del++;
					i++;
				}
			}
			print_edits(i, j, num_del, num_ins);
		}

		if(link[i][j] & DIAG)
		{
			trace.push_back(DIAG);
			report_difference(i-1, j-1);
		}else if(link[i][j] & VERT)
		{
			trace.push_back(VERT);
			report_difference(i-1, j);
		}else if(link[i][j] & HORZ)
		{
			trace.push_back(HORZ);
			report_difference(i, j-1);
		}

		trace.clear();
	};

	void print_edits(int del, int ins, int num_d, int num_i)
	{
		int orig_ins = ins - num_i;
		int orig_del = del - num_d;
		char type = ' ';
		//string comma = ",";
		string info_line;
		if(num_d > 0 || num_i > 0)
		{
			if((num_i > 0) && (num_d == 0))
			{
				type = 'a';
			}else if((num_i == 0) && (num_d > 0))
			{
				type = 'd';
			} else
			{
				type = 'c';
			}
			if(orig_del == del || orig_del+1 == del)
			{
				cout << del << type;
				if(orig_ins == ins || orig_ins+1 == ins)
				{
					cout << ins << endl;
				}else
				{
					cout << orig_ins+1 << "," << ins << endl;
				}
			}else if (orig_ins == ins || orig_ins+1 == ins)
			{
				cout << orig_del+1 << "," << del << type << ins << endl;
			}
			else
			{
				cout << orig_del+1 << "," << del << type << orig_ins+1 << "," << ins << endl;
			}
			//cout << info_line << endl;
			for(int j = orig_del+1; j <= del; j++)
			{
				cout << "< " << text1[j] << endl;
			}
			if(type=='c')
			{
			cout << "---" << endl;
			}
			for(int i = orig_ins+1; i <= ins; i++)
			{
				cout << "> " << text2[i] << endl;
			}
		}
	}

	void print_verbose()
	{
		cout << "cost matrix:" << endl;
		for(int i = 0; i <=m; i++)
		{
			for(int j = 0; j <= n; j++)
			{
				cout << setw(4) << right << cost[i][j];
			}
			cout << endl;
		}
		cout << "link matrix:" << endl;
		for(int i = 0; i <=m; i++)
		{
			for(int j = 0; j <= n; j++)
			{
				cout << setw(4) << right << link[i][j];
			}
			cout << endl;
		}
	}
  private:
	// support functions
    // text1 buffer
	vector<string> text1;
    // text2 buffer
	vector<string> text2;
	vector<int> trace;
	matrix<int> cost;
	matrix<int> link;
	int VERT = 1; // 1 -- see below
    int HORZ = 2; // 2 -- see below
    int DIAG = 4; // 4 -- see below

	int m, n;
	string ex, ey;

	int DEL(string c) { return 1; }
    int INS(string c) { return 1; }
    int SUB(string c1, string c2) { return c1.compare(c2)==0 ? 0 : INT_MAX/2; }
};

int main(int argc, char *argv[])
{
  // check two input files are specified on command line
  string usage = "usage: ./Diff7 [-verbose] file1 file2";
	if (argc < 3 || argc > 4){
		cerr << usage << endl;
		return 1;
	}

	bool verbose;
	ifstream orig;
	ifstream modif;
	if(string(argv[0]) != "./Diff7"){
		cerr << usage << endl;
		return 1;
	} 
	if (string(argv[1]) == "-verbose"){
		//cout << "encode is true" << endl;
		verbose = true;
	}
	if(verbose)
	{
		orig.open(argv[2]);
		modif.open(argv[3]);
	}else
	{
		orig.open(argv[1]);
		modif.open(argv[2]);
	}
	

  LCS lcs;  // instantiate your "LCS based diff" object

  // read the text from file1 into the lcs.text1 buffer
	string str;
	int m = 0;
	int n = 0;
	while(getline(orig, str))
	{
		lcs.text1_push_back(str);
		//cout << "pushed back orig: " << str << endl;
		m++;
	}
  // read the text from file2 into the lcs.text2 buffer
  while(getline(modif, str))
	{
		lcs.text2_push_back(str);
		//cout << "pushed back modif: " << str << endl;
		n++;
	}

	//cout << "read stuff in" << endl;

  lcs.compute_alignment();
  //cout << "computed alighment" << endl;
  //cout << "m: " << m << " n:" << n << endl;
  if(verbose)
  {
	lcs.print_verbose();
	//cout << "printed verbose" << endl;
  }
  lcs.report_difference(m, n);
}
