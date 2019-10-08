#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>	
using namespace std;

#include "Support.h"

const char ETX = 0x3;

void ppm::new_image(int w, int h)
{
	if(image!= NULL)
	{
		delete image;
	}
	image = new matrix<RGB>(w,h);
	//cout << "[Support.cpp]: new image created" << endl;
}

void ppm::read(const char *fname) { 
	ifstream fin;
	fin.open(fname);

	if(fin.is_open())
	{
		fin >> magicid >> w >> h >> maxvalue;	
	}
	if(magicid.compare("P6") != 0)
	{
		cout << "It's not P6" << endl;
	}
	if(maxvalue != 255){
		cout << "maxvalue is " << maxvalue << endl;
	}

	//int expected_bytes = w*h*3;
	//int actual_bytes = 0;
	int gottten = fin.get();
	new_image(w, h);
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{ 		
			RGB &ref = (*image)[j][i];
			//cout << "[Support.cpp]: RGB ref object is created" << endl;
			//cout << fin.get() << endl;
			ref.R = fin.get();
			ref.G = fin.get();
			ref.B = fin.get();
			//cout << "test" << endl;
			//actual_bytes += fin.gcount()*3;
		}
	}

	/*if(expected_bytes != actual_bytes){
		cout << "Expected bytes: " << expected_bytes << " Actual bytes: " << actual_bytes << endl;
	}*/

	fin.close();


	//cout << "byte gotten: " << gottten << endl;

}

void ppm::write(const char *fname) {
	//cout << "started writing file" << endl;
	string filename = string(fname);
	string str = "_wmsg";
	//cout << filename << endl;
	filename.insert(filename.length()-4, str);
	//cout << filename << endl;
	ofstream fout;
	remove(filename.c_str());
	fout.open(filename.c_str());
	//fout.open("test.ppm");
	if(fout.is_open())
	{
		fout << magicid << "\n";
		fout << get_Ncols() << " " << get_Nrows() << "\n";
		fout << maxvalue << "\n";
	}
	else
	{
		cerr << "error opening file to write" << endl;
	}
	for(int i = 0; i < get_Nrows(); i++)
	{	
		for(int j = 0; j < get_Ncols(); j++)
		{
			RGB &ref = (*image)[j][i];
			fout << ref.R << ref.G << ref.B;
		}
	}
	//fout << "\n";
	//cout << "Done writing file" << endl;

	fout.close();

}
rnumgen::rnumgen(int seedvalue, vector<int> &v) 
{  
	srand(seedvalue);  
	F.resize(v.size());  
	partial_sum(v.begin(), v.end(), F.begin());  
	transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), F.back()));
}
int rnumgen::rand() 
{  
	const double randmax = RAND_MAX+1.0;
	const double p = (double)std::rand()/randmax;  
	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
