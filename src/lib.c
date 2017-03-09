#include "../include/lib.h"

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
#define STARTCHAR 'S'
#define TARGETCHAR 'T'
#define WALLCHAR '@'
#define ROADCHAR '.'
#define SIDEWALL '+'
#define putcolored(C, BACK, FORE)			\
  printf("\033[%d;%dm %c \033[%d;%dm", FORE, BACK,	\
	 C, FWHITE, BBLACK)
#define putside() putcolored(SIDEWALL, FMAGENTA, BWHITE)

  index i, j;		/* out counters */
  
  for(i = 0; i < m->xdim; ++i) {
    putside(); /* print a sidewall each time we begin a new line */
    for(j = 0; j < m->ydim; ++j) {
      char nodeval;			     /* hold the node value */
      bool isfirsttime = (i == 0 && j == 0), /* when to print sidewalls */
	islasttime = (i == (m->xdim - 1) && j == (m->ydim - 1));
      bool wall = m->map[i][j].iswall;
      uint xstart = m->area[START].x, ystart = m->area[START].y,
	xtarget = m->area[TARGET].x, ytarget = m->area[TARGET].y;
      
      /* if the node is the starting/target node */
      if(i == xstart &&  j == ystart) nodeval = STARTCHAR;
      else if (i == xtarget && j == ytarget) nodeval = TARGETCHAR;
      /* determinate the `nodeval` based on its type */
      else if(wall) nodeval = WALLCHAR;
      else nodeval = ROADCHAR;
      
      /* print the upper-bar */
      if(isfirsttime) {
	/* the bar */
	for(index k = 0; k < m->xdim; ++k) 
	  putcolored('+',  FMAGENTA, BWHITE);
	  /* putside(); */
	/* sidewalls */
	putside(), putchar('\n');
	putside();
      }
      
      /* vary the node-color based on its type */
      switch(nodeval) {
      case WALLCHAR: putcolored(nodeval, FBLUE, BWHITE); break;
      case STARTCHAR: putcolored(nodeval, FBLACK, BGREEN); break;
      case TARGETCHAR: putcolored(nodeval, FBLACK, BYELLOW); break;

      default: putcolored(nodeval, FBLACK, BWHITE); break;
      };

      /* print the bottom-bar */
      if(islasttime) {
	putside(), putchar('\n');
	putside();
	for(index k = 0; k < m->xdim; ++k) 
	  putcolored('+',  FMAGENTA, BWHITE);
	  /* putside(); */
      }
    }
    putside();
    putchar('\n');
  }
#undef putcolored
#undef putside
#undef STARTCHAR 
#undef TARGETCHAR 
#undef WALLCHAR 
#undef ROADCHAR
#undef SIDEWALL
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

#ifdef DEBUG    
    printf("\ncurrent(%d, %d)\n",
	   current.x, current.y);
#endif

    break;
  }

}

void
mfree(maze_t *m)
{
  index i, j;
  /* WHY THE FUCK WOULD SOMEONE PUT
   * if(!p) {..} INSTEAD OF if(p)!!! */
  if(m) {    
#ifdef MEM_DEBUG
    printf("freeing.. ");
    printf("m:\t %p\n----\n", m);
#endif
    for(i = 0; i < m->ydim; ++i) {
      for(j = 0; j < m->ydim; ++j) {
#ifdef MEM_DEBUG
	printf("freeing.. ");
	printf("m->map[%d][%d].cord:\t %p | .parent:\t %p\n", 
	       i, j, m->map[i][j].cord, m->map[i][j].parent);
#endif
	free(m->map[i][j].cord);
	free(m->map[i][j].parent);
      }
#ifdef MEM_DEBUG
      printf("\n----\n", m);
#endif
    }
    for(i = 0; i < m->ydim; ++i) {
#ifdef MEM_DEBUG
      printf("freeing.. ");
      printf("m->map[%d]:\t %p\n",i, m->map[i]);
#endif
      free(m->map[i]);
    }
#ifdef MEM_DEBUG
    printf("freeing.. ");
    printf("m->map:\t %p\n", m->map);
#endif
    free(m->map);
#ifdef MEM_DEBUG
    printf("freeing.. ");
    printf("m->area:\t %p\n", m->area);
#endif
    free(m->area);
#ifdef MEM_DEBUG
    printf("freeing.. ");
    printf("m:\t %p\n", m);
#endif
    free(m);
  }
}
