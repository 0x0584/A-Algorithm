/* note to self:
 *
 * you vs time; who would be the winner? 
 * trust me, you don't really what would happen 
 * if you lose; an undefined behavior!
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* foreground colors */
#define FBLACK	 30
#define FWHITE   37

#define FRED	 31
#define FGREEN	 32
#define FYELLOW  33
#define FBLUE	 34
#define FMAGENTA 35
#define FCYAN	 36

/* background colors */
#define BBLACK	 40
#define BWHITE   47

#define BRED	 41
#define BGREEN	 42
#define BYELLOW  43
#define BBLUE	 44
#define BMAGENTA 45
#define BCYAN	 46


/* maze dimensions */
#define XDIM 20
#define YDIM XDIM

#define START 0
#define TARGET 1
#define POINTS_COUNT 2

#define FVALUE(x, y) (x + y)

#define WALLS 2

typedef enum BOOL {
  false = (1==0),
  true = !(false)
} bool;

typedef unsigned int index;
typedef unsigned int uint;

typedef struct COORDINATE {
  uint x; 			/* x-axis */
  uint y;			/* y-axis */
} cord_t;

typedef struct NODE {
  cord_t *cord,			/* coordinate of the node */
    *parent;			/* parent of the node */
  bool iswall;			/* is wall: true | false */
  uint mvcost,			/* G value is vary between 
				 * horizontal/vertical directions*/
    heuristic;			/* H value */
  bool isopen;  		/* if true, we can take consider
				 * this node as a future pathway */
  /* for performance reasons; avoiding 
   * decimals! using a whole number is 
   * a lot faster.. */
  #define G_HV 10		/* this is actually 1 */
  #define G_DIAG 14		/* this is actually sqrt(2) */
} node_t;

typedef struct MAZE {
  node_t **map;			/* maze map */
  uint xdim, ydim;		/* map dimensions */

  cord_t *area;			/* starting and target area */
} maze_t;

/* functions protoype */
maze_t *initmaze(void);
node_t **initmap(uint __xdim, uint __ydim, /* map dimensions */
		 cord_t *__area);	   /* set of points */
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

maze_t *
initmaze(void) 
{
  maze_t *foomaze = NULL;

  /* allocate memory */
  foomaze = (maze_t *) malloc(sizeof(cord_t));

  /* get maze dimensions */
  foomaze->xdim = XDIM;
  foomaze->ydim = YDIM;

  foomaze->area = (cord_t *) malloc(POINTS_COUNT*sizeof(cord_t));

  /* setup the maze map */
  foomaze->map = initmap(foomaze->xdim, foomaze->ydim,
			 foomaze->area);  

  return foomaze;
}

node_t **
initmap(uint x, uint y, cord_t *a)
{
  node_t **m;		/* our map */
  index i, j;
  
  /* allocate memory */
  m = (node_t **) malloc(x * sizeof(node_t *));
  
  for(i = 0; i < x; ++i) {
    
    /* allocate memory */
    m[i] = (node_t *) malloc(y * sizeof(node_t));    

    /* fill the map */
    for(j = 0; j < y; ++j) {
      /* allocate memory, always.. */
      m[i][j].cord = (cord_t *) malloc(sizeof(cord_t));
      m[i][j].parent = (cord_t *) malloc(sizeof(cord_t));
      
      /* get coordinates */
      m[i][j].cord->x = i;
      m[i][j].cord->y = j;

      /* set node's parent */
      m[i][j].parent = NULL;

      /* node type */
      m[i][j].iswall = !(rand()%WALLS) ? true : false;

      /* init G/H cost */
      m[i][j].mvcost = m[i][j].heuristic = 0;

      m[i][j].isopen = false; 
    }
  }

  /* set random coordinates for the starting-area */
  a[START].x = rand()%x;
  a[START].y = rand()%y;

  /* set random coordinates for the  target-area */
  a[TARGET].x = rand()%x;
  a[TARGET].y = rand()%y;
  
  return m;
}

void
putmaze(maze_t *m)
{
  index i, j;		/* out counters */
  
  for(i = 0; i < m->xdim; ++i) {
    for(j = 0; j < m->ydim; ++j) {

      /* temporary variables */
      bool wall = m->map[i][j].iswall;
      uint xstart = m->area[START].x, ystart = m->area[START].y,
	xtarget = m->area[TARGET].x, ytarget = m->area[TARGET].y;
      char nodeval;	/* hold the node value */

#define STARTCHAR 'S'
#define TARGETCHAR 'T'
#define WALLCHAR '#'
#define ROADCHAR '.'
      
      /* if the node is the starting/target node */
      if(i == xstart &&  j == ystart) nodeval = STARTCHAR;
      else if (i == xtarget && j == ytarget) nodeval = TARGETCHAR;
      /* determinate the `nodeval` based on its type */
      else if(wall) nodeval = WALLCHAR;
      else nodeval = ROADCHAR;
      /*
      for(int i = 20; i < 50; ++i) {
	printf("i:%d\n", i);
	for(j = 20; j < 50; ++j)
	  printf("\033[%d;%dmj:\t%d \033[0;37m ",i, j,j);
	//      getchar();
        printf("\n\n\n");
      }
      */
#define putcolored(C, BACK, FORE) \
      printf("\033[%d;%dm %c \033[%d;%dm", FORE, BACK,	\
	     C, FWHITE, BBLACK)
      
      switch(nodeval) {
      case WALLCHAR: putcolored(nodeval, FBLUE, BWHITE); break;
      case STARTCHAR: putcolored(nodeval, FBLACK, BGREEN); break;
      case TARGETCHAR: putcolored(nodeval, FBLACK, BRED); break;
      default: putcolored(nodeval, FBLACK, BWHITE); break;
      };
    }    
    putchar('\n');
  }
}

void
seek(maze_t *m, cord_t *a)
{
#define POSSIBLE_MOVES 8 /* no limit on the moves (e.g. no diagonal) */
  
#define LEFT(w, x, y) w[x][y - 1]
#define RIGHT(w, x, y) w[x][y + 1]
#define UP(w, x, y) w[x - 1][y]
#define DOWN(w, x, y) w[x + 1][y]
#define UPLEFT(w, x, y) w[x - 1][y - 1]
#define DOWNLEFT(w, x, y) w[x + 1][y - 1]
#define UPRIGHT(w, x, y) w[x - 1][y + 1]
#define DOWNRIGHT(w, x, y) w[x + 1]

  /* local functions */  
  bool iswalkable(node_t *__node, cord_t *__parent) {
    return !(__node->iswall) ? ( __node->parent = __parent), false : true;
  };
  cord_t nextstep(node_t **__map,
		  const cord_t __node) {
    cord_t foocord;

    
    
    return foocord;
  };

  /* local variables */
  index i, j,		            /* our counters */
    sizeofmaze;			    /* # of nodes in the maze */
  uint xtarget = a[TARGET].x,
    ytarget = a[TARGET].y;
  cord_t **path;
  bool isfirsttime = true;	    /* this one is to tell whether:
				     * (we are at the starting area) ||
				     * (on our way through the maze)
				     */

  /* function body */
  sizeofmaze = m->xdim * m->ydim;   /* number of nodes */

  /* allocate memory for the map */
  path = (cord_t **) malloc(sizeofmaze * sizeof(cord_t *));

  for(i = 0; i < sizeofmaze; ++i)
    path[i] = NULL;

  /* TODO:
   * 
   * this is a temporary version of this
   * function! i have to find out what's
   * the problem with realloc()!
   */

  bool isthereopen(maze_t *m) {
    index i;
    
    for(i = 0; i < m->xdim; ++i)
      for(j = 0; j < m->ydim; ++j)
	if(m->map[i][j].isopen == true) return true;

    return false;
  };
  /* seeking the target.. */
  while(isthereopen(m) ^ isfirsttime) {
    cord_t current;		   /* the current node */

    if(isfirsttime) {
      
      isfirsttime = false;
      
      current = a[START];
    }
    else
      current = nextstep(m->map, current);
    
    printf("\nocount:\t%d\t\tccount:\t%d\ncurrent(%d, %d)\n",
	   current.x, current.y);
    break;
  }

  /* always free you memory.. */
}

void
mfree(maze_t *m)
{
  index i, j;
  
  if(!m) {    
    for(i = 0; i < m->ydim; ++i)
      for(j = 0; j < m->ydim; ++j) {
	free(m->map[i][j].cord);
	free(m->map[i][j].parent);
      }
    
    for(i = 0; i < m->ydim; ++i)
      free(m->map[i]);
    free(m->map);
    free(m->area);
    free(m);
  }
}
