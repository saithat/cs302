
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>
#include "dset.h"
#include "dset.cpp"

struct cell {
  int x,y;
  cell(int i=-1, int j=-1){
    x = i;
    y = j;
  }
};

void swap(cell &c1, cell &c2) { 
  cell tmp = c1;
  c1 = c2;
  c2 = tmp;
}

void randperm(cell arr[][2], int size) {
  for (int i=size-1; i>0; --i) {
    int tmp_r = rand() % (i+1);
    swap(arr[i][0], arr[tmp_r][0]);
    swap(arr[i][1], arr[tmp_r][1]);
  }
}

int main(int argc, char *argv[]) {

  if (argc != 4) {
	fprintf(stderr, "usage: ./Mazemake nrow ncol maze.txt\n");
    return 0;
  }

  srand(time(NULL));

  int Nrow = atoi(argv[1]);
  int Ncol = atoi(argv[2]);

  // Grid wall:     (i0,j0) | (i1,j1)
  // Horizontal wall: (i,j) | (i+1,j)
  // Vertical wall:   (i,j) | (i,j+1)
/*
  cell grid[Nrow-1][Ncol-1];
  for (int i = 0; i < Nrow; i++)
  {
    for(int j = 0; j < Ncol; j++)
    {
      grid[i][j] = cell(i, j);
    }
  }
  */
    //number of interior grid walls
  int N = ((Ncol - 1)*Nrow)+((Nrow - 1)*Ncol);
  //printf("interior walls: %d\n", N);
  
  cell wall[N][2];

  //populate wall[][] with interior grid walls
  //randomly perturb list order: swap based
int index = 0;
//vertical
for(int i = 0; i < Ncol-1; i++)
{
  for(int j = 0; j < Nrow; j++)
  {
    wall[index][0] = cell(i, j);
    wall[index][1] = cell(i+1, j);
    //printf("wall index: %d (%d, %d), (%d, %d)\n", index, wall[index][0].x,
    //wall[index][0].y, wall[index][1].x, wall[index][1].y);
    index++;
  }
}
//horizontal
//printf("vertical walls now\n");
for(int i = 0; i < Ncol; i++)
{
  for(int j = 0; j < Nrow-1; j++)
  {
    wall[index][0] = cell(i, j);
    wall[index][1] = cell(i, j+1);
    //printf("wall index: %d (%d, %d), (%d, %d)\n", index, wall[index][0].x,
    //wall[index][0].y, wall[index][1].x, wall[index][1].y);
    index++;
  }
}
randperm(wall, index);
//printf("Stuff has been randomly permuted.\n");
  //open output file for writing (argv[3])
  FILE* f;
  f = fopen(argv[3], "w");
  fprintf(f, "MAZE %d %d\n", Nrow, Ncol);
  //write MAZE, Nrow, Ncol header

  dset DS(Nrow*Ncol);
  int k;
  //printf("created DS\n");
  for (k=0; k<N; k++) {
    /*pair of cells given by wall[k][] are not 
    connected (belong to different disjoint sets),merge them
    */
    int ii = wall[k][0].x + (wall[k][0].y*Ncol);
    //printf("index: %d: ii: %d\n", k, ii);
    int jj = wall[k][1].x + (wall[k][1].y*Ncol);
    //printf("index: %d: jj: %d\n", k, jj);
    //printf("DS find results: ii: %d\n", DS.find(ii));
    //printf("jj: %d\n", DS.find(jj));
	  if (DS.find(ii) != DS.find(jj)) {
      //printf("index: %d, merging %d and %d\n", k, DS.find(ii), DS.find(jj));
      DS.merge(ii,jj);
      //printf("DS size: %d\n", DS.size());
      //printf("index: %d, merging %d and %d\n", k, DS.find(ii), DS.find(jj));
    }
	/*write wall to file in the form of the
	two pairs of cell indices (i0,j0) (i1,j1) */
    else
    {
      fprintf(f, "%3d%4d%4d%4d\n", wall[k][0].y, wall[k][0].x, wall[k][1].y, wall[k][1].x);
    }
    //all cells belong to the same set, break
    if (DS.size() == 1)
    {
      //printf("DS size is 1\n");
      break;
    }
  }
  k++;
  //printf("DS size: %d\n", DS.size());
  //printf("finished merging and writing to file\n");
  //write any remaining walls to file
  for(;k<N;k++)
  {
    fprintf(f, "%3d%4d%4d%4d\n", wall[k][0].y, wall[k][0].x, wall[k][1].y, wall[k][1].x);
  }

  //close output file

  fclose(f);
} 
