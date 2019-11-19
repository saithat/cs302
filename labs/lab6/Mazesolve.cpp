#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>

struct cell {
  int x,y;
  cell(int i=-1, int j=-1){
    y = i;
    x = j;
  }
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

bool solve(bool*** wall, bool** iswhite, cell *stack, int *stack_int, cell *S, cell *T)
{
  iswhite[S->x][S->y] = true;
  stack[*stack_int].x = S->x;
  stack[*stack_int].y = S->y;
  //printf("stack[stack_int]: %d %d\n", stack[*stack_int].y, stack[*stack_int].x);
  *stack_int+=1;
  //printf("stack_int: %d\n", *stack_int);
  //printf("stack[stack_int]: %d %d\n", stack[*stack_int].x, stack[*stack_int].y);
  if((S->x == T->x) && (S->y == T->y))
  {
    //printf("At sink!\n");
    return true;
  }

  cell to;
  //printf("set stack\n");
  enum direction d;
  for(int d = DIR_LEFT; d <= DIR_DOWN; d++)
  {
    if(wall[S->x][S->y][d] == true)
    {
      //printf("wall exists! Going to 'continue'\n");
      continue;
    }
    else if(d == DIR_LEFT)
    {
      to.x = S->x -1;
      to.y = S->y;
      //printf("Set x-1, dir_left\n");
    }
    else if(d == DIR_RIGHT)
    {
      to.x = S->x + 1;
      to.y = S->y;
    }
    else if(d == DIR_UP)
    {
      to.x = S->x;
      to.y = S->y - 1;
    }
    else if(d == DIR_DOWN)
    {
      to.x = S->x;
      to.y = S->y + 1;
    }
    //printf("creating to_x and to_y variables\n");
    //printf("created to_x and to_y variables\n");
    //printf("to_x and to_y: %d %d\n", to.x, to.y);
    //printf("%B\n", iswhite[to_x][to_y]);
    if ((iswhite[to.x][to.y]) == true)
    {
      //printf("Already visisted next node\n");
      continue;
    }
    //printf("to is not white\n");
    //printf("setting next node in stack\n");
    stack[*stack_int].x = to.x;
    stack[*stack_int].y = to.y;
    //printf("set stack\n");
    if (solve(wall, iswhite, stack, stack_int, &to, T))
    {
      return true;
    }
  }
  iswhite[S->x][S->y] = false;
  *stack_int = *stack_int - 1;
  //printf("failed stack_int: %d\n", *stack_int);
  //printf("deadend\n");
  return false;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
  fprintf(stderr, "usage: ./Mazesolve maze.txt path.txt\n");
	return 0;
  } 

  int Nrow, Ncol;
  //open input file for reading
  FILE *fp = fopen(argv[1], "r");

  //open output file for writing
  FILE *out = fopen(argv[2], "w");
  //determine Nrow,Ncol from input file
  fscanf(fp, "MAZE %d %d", &Nrow, &Ncol);
  /*
  create array of walls for each grid cell
  initialize to have boundary walls set and
  interior walls unset
  */

bool ***wall = new bool **[Ncol];
  bool **iswhite = new bool *[Ncol];
  for(int x = 0; x < Ncol; x++)
  {
    wall[x] = new bool *[Nrow];
    iswhite[x] = new bool [Nrow];
    for(int y = 0; y < Nrow; y++)
    {
      wall[x][y] = new bool[4]();
    }
  }

  //printf("created wall and iswhite\n");
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
//printf("set border to true\n");
  //read input file, set interior walls
   int x1, y1, x2, y2;
  while(fscanf(fp,"%d %d %d %d", &y1, &x1, &y2, &x2) == 4)
  {
    //printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", x1, y1, x2, y2);
    wall[x1][y1][ get_dir(x1, y1, x2, y2) ] = true;
    wall[x2][y2][ get_dir(x2, y2, x1, y1) ] = true;

  }

  //initalize DFS path computation

  cell source = cell(0,0);
  cell sink = cell(Nrow-1,Ncol-1);

  cell stack[Nrow*Ncol];
  //int stack_int = 0;
  int stack_size = 0;

  //carry out DFS source-to-sink computation
  bool success = solve(wall, iswhite, stack, &stack_size, &source, &sink);
  //printf("returned from solve\n");
  //write PATH, Nrow, Ncol header
  if(success)
  {
  fprintf(out, "PATH %d %d\n", Nrow, Ncol);
  //write cells on path to file
  for(int i = 0; i < stack_size; i++)
  {
    fprintf(out, "%d %d\n", stack[i].y, stack[i].x);
  }
  }

  //close open files
  fclose(fp);
  fclose(out);
}
