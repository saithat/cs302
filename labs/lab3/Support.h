#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <cstdio>
#include <iostream>

typedef unsigned char uchar;
extern const char ETX;

template <typename T>
class matrix {
	public:
		matrix(int n_Nrows, int n_Ncols){
			Nrows = n_Nrows;
			Ncols = n_Ncols;

			buf = new T *[Ncols];
			for(int i = 1; i<Ncols; i++)
				buf[i] = new T[Nrows];
		}

		~matrix(){
			delete [] buf[0];
			delete [] buf;
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }
		T * operator[] (int i) { return buf[i]; }
	}
	private:
		int Nrows, Ncols;
		T **buf;
};

struct pixel {
	int row, col;
	pixel() { row = 0; col=0;}
	pixel(int n_row, int n_col) { row = n_row; col = n_col;}
};

struct RGB {
	char R, G, B;
	RGB() { R=0; G = 0; B = 0;}
};

class ppm {
	public:
		ppm(){
			image = NULL;
		}
		void read(const char *fname);
		void write(const char *fname);

		T* operator[] (int i) { return (*image)[i]; }
		int get_Nrows() const { return image->get_Nrows(); }
		int get_Ncols() const { return image->get_Ncols(); }

	private:
		string magicid;
		int w, h, maxvalue;

		matrix<RGB> * image;
		void new_image(int n_w, int n_h);
};

#endif
