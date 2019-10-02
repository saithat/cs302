#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;

#include "support.h"

const char ETX = 0x3;

void ppm::read(const char *fname) { 
	RGB *p;
	ifstream fin;
	fin.open(fname);

	if(fin.is_open())
	{
		fin >> magicid >> w >> h >> maxvalue;	
	}
	if(magicid.compare("P6") != 0)
	{
		cout << "It's not P6" << endl;
		return false;
	}
	if(maxvalue != 255){
		cout << "maxvalue is " << maxvalue << endl;
		return false;
	}

	int expected_bytes = w*h*3;
	int actual_bytes = 0;
	image = new matrix<RGB>(h, w);
	for(int i = 0; i < image.get_Nrows; i++)
	{
		for(int j = 0; j < image.get_Ncols; j++)
		{ 		
			p = new RGB;
			fin >> p->R >> p->G >> p->B;
			actual_bytes += fin.gcount();
			image->buf[i][j] = *p;
		}
	}

	if(expected_bytes != actual_bytes){
		cout << "Expected bytes: " << expected_bytes << " Actual bytes: " << actual_bytes << endl;
	}

	fin.close();

}

void ppm::write(const char *fname) { 
	filename = std::string(fname);
	str = "wmsg";
	filename.insert(-4, str);
	fstream fout;
	fout.open(filename.c_str());
	if(fout.is_open())
	{
		fout << magicid << endl;
		fout << w << " " << h << endl;
		fout << maxvalue << endl;
	}
	else
	{
		cerr << "error opening file to write" << endl;
		return false;
	}
	for(int i = 0; i < image.get_Nrows; i++)
	{	
		for(int j = 0; j < image.get_Ncols; j++)
		{
			fout << image.buf[i][j]->R << " " <<  image.buf[i][j]->G << " " << image.buf[i][j]->B << " ";
		}
	}

	fout.close();

}
