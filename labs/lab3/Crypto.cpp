#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <bitset>

using namespace std;

#include "Support.h"

/*void set_pixel_list(ppm &image, vector<pixel> &list)
{
	for(int i = 0; i < image.get_Nrows(); i++)
	{
		for(int j = 0; j < image.get_Ncols(); j = j+2)
		{
			list.push_back(pixel(i, j));
		}
	}
}*/

vector<pixel> set_pixel_list(ppm &im, int key_len)
{
	vector<pixel> list;
	vector<int> histogram;
	int i, x, y, w, h, r30;
	short color;

	w = im.get_Ncols();
	h=im.get_Nrows();

	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			list.push_back(pixel(x,y));
		}
	}

	histogram.resize(1<<15,0);

	for(y=0; y<h; y++)
	{
		for(x=0;x<w;x++)
		{
			RGB &ref = im[x][y];

			color = (short){((ref.R>>3)<<10)|((ref.G>>3)<<5)|(ref.B>>3)};
			histogram[color]++;
		}
	}
	//cout << "key_len for rng: " << key_len << endl;
	rnumgen rng(key_len, histogram);

	for(i = (int) list.size()-1; i>0; --i){
		r30 = (rng.rand() << 15) | rng.rand();

		swap(list[i],list[r30%(i+1)]);
	}

	return list;
}
/*
void encode(const char *fname, ppm image) {
	vector<pixel> plist;
	set_pixel_list(image, plist);

	//for (int i =0 ; i < plist.size(); i++) cout << plist[i].row << " " << plist[i].col << endl;

	cout << "pixel list set" << endl;
	fstream fin;
	bool end = false;
	fin.open(fname);
	
	char c;
	int pcount = 0;
	int psize = plist.size();
	cout << "psize: " << psize << endl;
	while(end == false)
	{
		if(!cin.get(c))
		{
			end = true;
			c = 0x3;
			cout << "end is true and ETX is c" << endl;
		}
		cout << "c: " << bitset<8>(c) << endl;
		//cout << "end: " << end << endl;
		if(pcount >= psize)
		{
			cerr << "pcount > psize" << endl;
			exit(0);
		}
		for(int i = 0; i < 7; i++)
		{
			cout << "c: " << bitset<8>(c) << endl;
			char LSB = (c >> i) & 0x1;
			//cout << "LSB: " << LSB << endl;
			RGB &pxl = image[plist[pcount].row][plist[pcount].col];
			//cout << "Before: Red: " << bitset<8>(pxl.R) << " Green: " << bitset<8>(pxl.G) << " Blue: " << bitset<8>(pxl.B) << endl;
			//cout << " row: " << plist[pcount].row << " col: " << plist[pcount].col << endl;
			switch(pcount % 3)
			{
				case 0:
					pxl.R &= 0xFE;
					pxl.R |= LSB;
					//cout << "Red" << endl;
					break;
				case 1:
					pxl.G &= 0xFE;
					pxl.G |= LSB;
					//cout << "Green" << endl;
					break;
				case 2:
					pxl.B &= 0xFE;
					pxl.B |= LSB;
					//cout << "Blue" << endl;
					break;
				default:
					break;
			}
			//cout << "After: Red: " << bitset<8>(pxl.R) << " Green: " << bitset<8>(pxl.G) << " Blue: " << bitset<8>(pxl.B) << endl;
			//cout << endl;
			pcount++;
		}
	}
}
*/
void encode(ppm &im, const vector<pixel> &pixl, string key)
{
	char c;
	char k[key.length()+1];
	strcpy(k, key.c_str());
	int x, y, i_col, i_pix, i_bit, psz, klen, kcount;
	bool insert;

	i_col = 0;
	i_pix = 0;
	kcount=0;
	klen = key.length();

	//cout << "keylen: " << klen << " k size: " << sizeof(k) << endl;
	//cout << k << endl;
	insert = false;
	psz = pixl.size();

	while(insert == false){
		if(!cin.get(c)){
			c=ETX;
			insert = true;
		}
		c = c^k[kcount%klen];
		for(i_bit=0; i_bit < 7; i_bit++)
		{
			if(i_pix>=psz)
			{
				cerr <<"plist exhausted" << endl;
				exit(0);
			}
			x = pixl[i_pix].row;
			y = pixl[i_pix].col;

			//for (int i =0 ; i < pixl.size(); i++) cout << pixl[i].row << " " << pixl[i].col << endl;
			//cout << "last row and col: " << pixl[pixl.size()-1].row << " " << pixl[pixl.size()-1].col << endl;
			RGB &pxl = im[x][y];
			//cout << "c before: " << c << endl;
			//cout << "c after xor: " << c << endl;
			switch(i_col)
			{
				case 0:
					pxl.R &= 0xFE;
					pxl.R |= ((c >>i_bit) & 0x1);
					//cout << "Red" << endl;
					break;
				case 1:
					pxl.G &= 0xFE;
					pxl.G |= ((c >>i_bit) & 0x1);
					//cout << "Green" << endl;
					break;
				case 2:
					pxl.B &= 0xFE;
					pxl.B |= ((c >>i_bit) & 0x1);
					//cout << "Blue" << endl;
					break;
				default:
					break;
			}
			if(i_col == 2)	{i_col=0;}
			else			{i_col++;}
			i_pix++;
		}
		kcount++;
	}
}

/*void decode(const char *fname, ppm image) {
	vector<pixel> plist;
	set_pixel_list(image, plist);
	cout << "pixel list set" << endl;
	fstream fin;
	bool end = false;
	fin.open(fname);

	//for (int i =0 ; i < plist.size(); i++) cout << plist[i].row << " " << plist[i].col << endl;
	
	char c = 0;
	int pcount = 0;
	int psize = plist.size();
	cout << "psize: " << psize << endl;
	while(end == false)
	{
		c = 0;
		RGB &pxl = image[plist[pcount].row][plist[pcount].col];
		//cout << "Before: Red: " << bitset<8>(pxl.R) << " Green: " << bitset<8>(pxl.G) << " Blue: " << bitset<8>(pxl.B) << endl;
		//cout << " row: " << plist[pcount].row << " col: " << plist[pcount].col << endl;
		//cout << "Red: " << pxl.R << " Green: " << pxl.G << " Blue: " << pxl.B << endl;
		for(int i = 0; i < 7; i++)
		{
			switch(pcount % 3)
			{
				case 0:
					c |= ((pxl.R & 0x1) << i);
					//cout << "Red" << endl;
					break;
				case 1:
					c |= ((pxl.G & 0x1) << i);
					//cout << "Green" << endl;
					break;
				case 2:
					c |= ((pxl.B & 0x1) << i);
					//cout << "Blue" << endl;
					break;
				default:
					break;
			}
			//cout << "After: Red: " << bitset<8>(pxl.R) << " Green: " << bitset<8>(pxl.G) << " Blue: " << bitset<8>(pxl.B) << endl;
			//cout << endl;
			pcount++;
			if(c == 0x3)
			{
				end = true;
				cout << "end has been reached" << endl;
				break;
			}
		}
		c &= 0x7F;
		cout << c;
	}

}*/

void decode(ppm &im, const vector<pixel> &pixl, string key)
{
	char c;
	char k[key.length()+1];
	strcpy(k, key.c_str());
	int x, y, i_col, i_pix, i_bit, klen, kcount;

	i_col = 0;
	i_pix = 0;

	int t=0;
	klen = key.length();
	kcount = 0;

	//for (int i =0 ; i < pixl.size(); i++) cout << pixl[i].row << " " << pixl[i].col << endl;
	//cout << "last row and col: " << pixl[pixl.size()-1].row << " " << pixl[pixl.size()-1].col << endl;
	while(true)
	{
		c=0;
		for(i_bit=0; i_bit<7; i_bit++)
		{
			x = pixl[i_pix].row;
			y = pixl[i_pix].col;

			RGB &pxl = im[x][y];

			switch(i_col)
			{
				case 0:
					c |= ((pxl.R & 0x1) << i_bit);
					//cout << "Red" << endl;
					break;
				case 1:
					c |= ((pxl.G & 0x1) << i_bit);
					//cout << "Green" << endl;
					break;
				case 2:
					c |= ((pxl.B & 0x1) << i_bit);
					//cout << "Blue" << endl;
					break;
				default:
					break;
			}
			//cout << "c before: " << c << endl;
			//c = c^k[kcount%klen];
			//cout << "c after xor: " << c << endl;
			if(i_col == 2)	{i_col=0;}
			else			{i_col++;}
			i_pix++;
			//kcount++;
		}
		c = c^k[kcount%klen];
		kcount++;
		if(c==ETX)
		{
			break;
		}
		cout <<c;
	}
}

int main(int argc, char *argv[]) {
  
  string usage = "usage: ./Crypto -encode|decode image.ppm";
	if (argc < 3 || argc > 4){
		cerr << usage << endl;
		return 1;
	}

	bool enc = false;
	bool dec = false;
	string key = "";
	bool key_given = false;	
	if(string(argv[0]) != "./Crypto"){
		cerr << usage << endl;
		return 1;
	} 
	if (string(argv[1]) == "-encode"){
		//cout << "encode is true" << endl;
		enc = true;
	} 
	if (string(argv[1]) == "-decode"){
		//cout << "decode is true" << endl;
		dec = true;
	}
	if(string(argv[2]).substr(0,4) == "-key")
	{
		key = string(argv[2]).substr(5, string(argv[2]).length()-1);
		//cout << key;
		key_given = true;
	}

	char * fname;
	if(!key_given)
	{
  	fname = argv[2];
	}else
	{
	fname = argv[3];
	}
 // cout << "fname read file name" << endl;
  ppm img;
  vector<pixel> pixx;
  //cout << "created ppm img" << endl;
  img.read(fname);
 // cout << "passed fname to img.read()" << endl;
  pixx = set_pixel_list(img, key.length());

	img.write(fname);
  if (enc) 
  {
    encode(img, pixx, key);
	img.write(fname);
  }
  else if (dec)
  {
    decode(img, pixx, key);
  }
}
