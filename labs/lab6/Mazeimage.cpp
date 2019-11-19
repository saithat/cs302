#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>

struct RGB {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	RGB() { R=0; G=0; B=0;}
};

typedef enum direction {
  DIR_LEFT, /* 0 */
  DIR_UP, /* 1 */
  DIR_RIGHT, /* 2 */
  DIR_DOWN /* 3 */
  } DIR_T;

DIR_T get_dir(int x1, int y1, int x2, int y2)
{
  if (x1 == x2)
  {
    if(y1 < y2)
    {
      return DIR_DOWN;
    }
    return DIR_UP;
  }
  if(x1 < x2)
  {
    return DIR_RIGHT;
  }
  return DIR_LEFT;
}

int main(int argc, char *argv[]) {
	if ((argc < 2) || (argc > 4)){
		fprintf(stderr, "usage: ./Mazeimage maze.txt [path.txt]\n");
		return 1;
	}
	char n[256];
strncpy(n, argv[1], strlen(argv[1])-4);
strcat(n, ".ppm");

	FILE *fp = fopen(argv[1], "r");
	FILE *path_f;    
	FILE *out = fopen(n, "wb");
    bool solution_included = false;
	if (argc > 2)
	{
		path_f = fopen(argv[2], "r");
		solution_included = true;
		if(argc == 4)
		{
			fclose(out);
			out = fopen(argv[3], "wb");
		}
	}

	char magicid[3] = "P6";
  	int Ncol, Nrow;
  	int maxvalue = 255;
  //open input file for reading

  fscanf(fp, "MAZE %d %d", &Nrow, &Ncol);


  int cell_width = 10;
  int Ncol_head = (Ncol * cell_width) + 1;
  int Nrow_head = (Nrow * cell_width) + 1;


  fprintf(out, "%s\n%d %d\n%d\n", magicid, Ncol_head, Nrow_head, maxvalue);
  

  bool wall[Ncol][Nrow][4];

	for(int i = 0; i < Ncol; i++)
  {
    for(int j = 0; j < Nrow; j++)
    {
      if(i==0)
      {
        wall[i][j][DIR_LEFT] = true;
      }
      if(i==(Ncol-1))
      {
        wall[i][j][DIR_RIGHT] = true;
      }
      if(j == 0)
      {
        wall[i][j][DIR_UP] = true;
      }
      if( j == (Nrow-1))
      {
        wall[i][j][DIR_DOWN] = true;
      }
    }
  }
	//read in interior walls
  int x1, y1, x2, y2;
  while(fscanf(fp,"%d %d %d %d", &y1, &x1, &y2, &x2) == 4)
  {

    wall[x1][y1][ get_dir(x1, y1, x2, y2) ] = true;
    wall[x2][y2][ get_dir(x2, y2, x1, y1) ] = true;

  }

	RGB background;
  background.R = 255;
  background.G = 255;
  background.B = 255;
  RGB sol;
  sol.R = 143;
  sol.G = 188;
  sol.B = 143;
  RGB wall_c;
  wall_c.R = 0;
  wall_c.G = 0;
  wall_c.B = 0;

	// +2 for pixel border around each cell
  RGB image[Ncol][Nrow][cell_width+2][cell_width+2];

  for(int i = 0; i < Ncol; i++)
  {
	  for(int j = 0; j < Nrow; j++)
	  {
		  for(int k = 0; k <cell_width+2; k++)
		  {
			  for(int l = 0; l <cell_width+2; l++)
			  {
				  image[i][j][k][l].R = background.R;
				  image[i][j][k][l].G = background.G;
				  image[i][j][k][l].B = background.B;
			  }
		  }
	  }
  }


  if(solution_included)
  {
	int Prow, Pcol;
	fscanf(path_f, "PATH %d %d", &Prow, &Pcol);
	int Px, Py;
	while(fscanf(path_f, "%d %d", &Py, &Px)==2)
	{

		for(int i=1; i< cell_width+1; i++)
		{
			for(int j=1; j<cell_width+1; j++)
			{

				image[Py][Px][i][j].R = sol.R;
				image[Py][Px][i][j].G = sol.G;
				image[Py][Px][i][j].B = sol.B;

			}
		}
	}
  }

// walls

  for(int i = 0; i < Ncol; i++)
  {
	  for(int j = 0; j < Nrow; j++)
	  {
		  for(int d = DIR_LEFT; d <= DIR_DOWN; d++)
		  {
			if(wall[i][j][d] == true)
			{

				if((d == DIR_LEFT))
				{
					for(int k=0; k < cell_width+2; k++)
					{
						image[i][j][0][k].R = wall_c.R;
						image[i][j][0][k].G = wall_c.G;
						image[i][j][0][k].B = wall_c.B;
					}
				}
				else if(d == DIR_DOWN)
				{
					for(int k = 0; k < cell_width+2; k++)
					{
						image[i][j][k][cell_width+1].R = wall_c.R;
						image[i][j][k][cell_width+1].G = wall_c.G;
						image[i][j][k][cell_width+1].B = wall_c.B;
					}
				}
				else if(d == DIR_RIGHT)
				{
					for(int k = 0; k < cell_width+2; k++)
					{
						image[i][j][cell_width+1][k].R = wall_c.R;
						image[i][j][cell_width+1][k].G = wall_c.G;
						image[i][j][cell_width+1][k].B = wall_c.B;
					}
				}
				else if(d == DIR_UP)
				{
					for(int k = 0; k < cell_width+2; k++)
					{
						image[i][j][k][0].R = wall_c.R;
						image[i][j][k][0].G = wall_c.G;
						image[i][j][k][0].B = wall_c.B;
					}
				}
			}
		  }
	  }
  }

  
  for(int i = 0; i < Ncol; i++)
  {
	  for(int j = 0; j < Nrow; j++)
	  {
		  for(int k = 0; k <cell_width+2; k++)
		  {
			  for(int l = 0; l <cell_width+2; l++)
			  {

				  fwrite(&image[i][j][k][l].R, 1, 1, out);
				  fwrite(&image[i][j][k][l].G, 1, 1, out);
				  fwrite(&image[i][j][k][l].B, 1, 1, out);
			  }
		  }
	  }
  }
  //fwrite(image, 1, 3, out);
  //printf("write to output\n");
  fclose(out);
  fclose(fp);
  fclose(path_f);
}