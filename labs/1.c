#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* maze dimensions */
#define XDIM 8
#define YDIM XDIM

#define START 0
#define TARGET 1
#define POINTS_COUNT 2

#define WALLS 5

typedef enum BOOL {
  false = (1==0),
  true = !(false)
} bool;

typedef struct COORDINATE {
  unsigned int x, y;
} cord_t;

typedef struct NODE {
  cord_t *cord;			/* coordinate of the node */
  bool iswall;			/* is wall: true | false */
} node_t;

typedef struct MAZE {
  node_t **map;			/* maze map */
  unsigned int xdim, ydim;	/* map dimensions */

  cord_t *area;			/* starting and target area */
  
} maze_t;

/* functions protoype */
maze_t *initmaze(void);
node_t **initmap(unsigned int __xdim, unsigned int __ydim,
		 cord_t *__area);
void putmaze(maze_t *__maze);
void mfree(maze_t *__maze);
void seek(maze_t *__maze, cord_t *__area);

/* the main function */
int
main()
{
  puts("$");
  srand(time(NULL));

  maze_t *maze = initmaze();
  
  printf("START(%d, %d) -> TARGET(%d, %d)\n",
	 maze->area[START].x, maze->area[START].y,
	 maze->area[TARGET].x, maze->area[TARGET].y);


  putmaze(maze);

  seek(maze, maze->area);
  
  mfree(maze);
  
  return 0;
}


maze_t *initmaze(void) 
{
  maze_t *foomaze = NULL;
  
  foomaze = (maze_t *) malloc(sizeof(cord_t));

  /* get maze dimensions */
  foomaze->xdim = XDIM;
  foomaze->ydim = YDIM;

  foomaze->area = (cord_t *) malloc(POINTS_COUNT*sizeof(cord_t));

  /* setup the maze map */
  foomaze->map = initmap(foomaze->xdim, foomaze->ydim, foomaze->area);  

  return foomaze;
}

node_t **
initmap(unsigned int x, unsigned int y, cord_t *a)
{
  node_t **m;		/* our map */
  
  /* allocate memory */
  m = (node_t **) malloc(x * sizeof(node_t *));
  
  for(unsigned int i = 0; i < x; ++i) {
    
    /* allocate memory */
    m[i] = (node_t *) malloc(y * sizeof(node_t));    

    /* fill the map */
    for(unsigned int j = 0; j < y; ++j) {
      /* allocate memory, always.. */
      m[i][j].cord = (cord_t *) malloc(sizeof(cord_t));

      /* get coordinates */
      m[i][j].cord->x = i;
      m[i][j].cord->y = j;
      m[i][j].iswall = !(rand()%WALLS) ? true : false;
    }
  }

  /* set a random starting-area */
  a[START].x = rand()%x;
  a[START].y = rand()%y;

  /* set a random target-area */
  a[TARGET].x = rand()%x;
  a[TARGET].y = rand()%y;
  
  return m;
}

void
putmaze(maze_t *m)
{
  unsigned int i, j;		/* out counters */
  
  for(i = 0; i < m->xdim; ++i) {
    for(j = 0; j < m->ydim; ++j) {
      /* temporary variables */
      bool wall = m->map[i][j].iswall;
      unsigned int xstart = m->area[START].x,
	ystart = m->area[START].y,
	xtarget = m->area[TARGET].x,
	ytarget = m->area[TARGET].y;

      char nodeval;		/* hold the node value */
      
      if(xstart == i &&  ystart == j) nodeval = 'S';
      else if (xtarget == i && ytarget == j) nodeval = 'T';
      else if(wall) nodeval = '#';
      else nodeval = '.';

      putchar(nodeval);
    }
    putchar('\n');
  }
}

void
mfree(maze_t *m)
{
  if(!m) {    
    for(unsigned int i = 0; i < m->ydim; ++i)
      for(unsigned int j = 0; j < m->ydim; ++j)
	free(m->map[i][j].cord);
    
    for(unsigned int i = 0; i < m->ydim; ++i)
      free(m->map[i]);

    free(m->map);
    free(m->area);
    free(m);
  }
}
