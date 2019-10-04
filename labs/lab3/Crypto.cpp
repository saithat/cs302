#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include "Support.h"

void encode(const char *fname, ppm image) {
	vector<pixel> plist;
	set_pixel_list(image, plist);
	fstream fin;
	fin.open(fname);
	

}

void decode(const char *fname, ppm image) {

}

void set_pixel_list(ppm &image, vector<pixel> &list)
{
	for(int i = 0; i < image.get_Nrows; i++)
	{
		for(int j = 0; j < image.get_Ncols; j = j+2)
		{
			list.push_back(pixel(i, j));
		}
	}
}

int main(int argc, char *argv[]) {
  
  string usage = "usage: ./Crypto -encode|decode image.ppm";
	if (argc != 3){
		cerr << usage << endl;
		return 1;
	}

	bool enc = false;
	bool dec = false;	
	if(string(argv[0]) != "./Crypto"){
		cerr << usage << endl;
		return 1;
	} 
	if (string(argv[1]) == "-encode"){
		enc = true;
	} 
	if (string(argv[1]) == "-decode"){
		dec = true;
	}
  char * fname = argv[2];
  ppm img;
  img.read(fname);

  if (enc) 
  {
    encode(fname, img);
  }
  else if (dec)
  {
    decode(fname, img);
  }
}
