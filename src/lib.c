#include <math.h>

#include "../include/lib.h"
#include "../include/stack.h"

static inline float trav(cord_t *a, cord_t *b);

maze_t *
initmaze(void) 
{
  puts("begin initmaze");
  
  maze_t *foomaze = NULL;
  node_t *foostart, *footarget;

  /* allocate memory */
  foomaze = (maze_t *) malloc(sizeof(maze_t));

  /* get maze dimensions */
  foomaze->xdim = XDIM;
  foomaze->ydim = YDIM;

  foostart = (node_t *) malloc(sizeof(node_t));
  foostart->cord = (cord_t *) malloc(sizeof(cord_t));
  foostart->parent = NULL;
    
  footarget = (node_t *) malloc(sizeof(node_t));
  footarget->cord = (cord_t *) malloc(sizeof(cord_t));
  footarget->parent = NULL;

  /* set random coordinates for the starting-area */
  footarget->cord->x = rand()%XDIM; 
  footarget->cord->y = rand()%YDIM;
  
  /* set random coordinates for the target-area */
  foostart->cord->x = rand()%XDIM; 
  foostart->cord->y = rand()%YDIM;


  foomaze->start = foostart;
  foomaze->target = footarget;

  printf("start(%d, %d) target(%d, %d)\n",
	 foomaze->start->cord->x,foomaze->start->cord->y,
	 foomaze->target->cord->x,foomaze->target->cord->y);
  
  /* setup the maze map */
  foomaze->map = (node_t ***) initmap(foomaze->xdim, foomaze->ydim, footarget);
  
  puts("maze init is good");

  return foomaze;
}

node_t ***
initmap(uint x, uint y, node_t *to)
{
  puts("begin initmap");
  
  node_t ***m;		/* our map */
  index i, j;
  
  /* allocate memory */
  m = (node_t ***) malloc(x * sizeof(node_t **));

  printf("m: %p\n", m);
  
  for(i = 0; i < x; ++i) {    
    /* allocate memory */
    m[i] = (node_t **) malloc(y * sizeof(node_t *));    
    printf("m[%d]: %p\n", i, m[i]);
  
    /* fill the map */
    for(j = 0; j < y; ++j) {
      printf("m[%d][%d]: %p\n", i, j, m[i][j]);
      m[i][j] = (node_t *) malloc(sizeof(node_t ));
      
      /* allocate memory, always.. */
      m[i][j]->cord = (cord_t *) malloc(sizeof(cord_t));
      m[i][j]->parent = (node_t *) malloc(sizeof(node_t));
      
      /* get coordinates */
      m[i][j]->cord->x = i;
      m[i][j]->cord->y = j;

      /* set node's parent */
      m[i][j]->parent = NULL;

      /* node type */
      m[i][j]->iswall = !(rand()%WALLS) ? true : false;

      /* init G/H cost */
      m[i][j]->mvcost = 0;
      m[i][j]->heuristic = trav(m[i][j]->cord, to->cord);
      m[i][j]->state = UNCHECKED;
    }
  }

    puts("end initmap");
  return m;

}

static inline float
trav(cord_t *a, cord_t *b) {
    float deltaX = b->x - a->x;
    float deltaY = b->y - a->x;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
};

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
      bool wall = m->map[i][j]->iswall;
      int state = m->map[i][j]->state;
      uint xstart = m->start->cord->x, ystart = m->start->cord->y,
	xtarget = m->target->cord->x, ytarget = m->target->cord->y;
      
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
      case STARTCHAR: putcolored(nodeval, FBLACK, BYELLOW); break;
      case TARGETCHAR: putcolored(nodeval, FBLACK, BRED); break;

      default:
	if(state == OPEN) putcolored((nodeval = '*'), FRED, BBLUE); 
	else if(state == CLOSED) putcolored((nodeval = '*'), FBLUE, BRED);
	else if(state == IN_PATH) putcolored((nodeval = '#'), FCYAN, BGREEN);
	else putcolored((nodeval = '.'), FMAGENTA, BWHITE);
	
	break;
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

#define DIRECTIONS 8

cord_t **
adjacents(cord_t *c, uint xdim, uint ydim)
{  
  cord_t **foocord = (cord_t **) calloc(DIRECTIONS, sizeof(cord_t *));

  short directions[][2] = {{-1, -1}, {-1, 0}, {-1, 0},{0, 1},
		     {1, 1}, {1, 0}, {1, -1}, {0, -1}};
  

  int i, nonpossible = 0;

  for(i = 0; i < DIRECTIONS; ++i) {
    int x = c->x + directions[i][0],
      y = c->y + directions[i][1];
    
    /* Stay within the grid's boundaries */
    if(x < 0 || x >= (signed) xdim ||
       y < 0 || y >= (signed) ydim)
      { nonpossible++; continue; };
    
    foocord[i - nonpossible]->x = x;
    foocord[i - nonpossible]->y = y;
  };

  return (foocord = realloc(foocord, sizeof(cord_t *) - nonpossible));
}

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count)
{  
  node_t **walkable = NULL;
  index i, wcount = 0;		/* count of walkable nodes */

  /* get list of possible nodes */
  cord_t **next = adjacents(n->cord, xdim, ydim);
  puts("initneighbors");

  for(i = 0; i < DIRECTIONS; ++i) {
      uint x = next[i]->x;
      uint y = next[i]->y;

      if(!next[i]) continue; /* next is NULL */
      
      node_t *node = m->map[x][y];
      
      /* ignore non-walkable nodes */
      if (!node->iswall) continue;

      /* ignore already-closed nodes */
      if (node->state == CLOSED) continue;

      /* Already-open nodes are only added to the
       * list if their G-value is lower going via
       * this route. */
      if (node->state == OPEN) {
	float traversalcost = trav(node->cord, node->parent->cord);
	float gtemp = n->mvcost + traversalcost;

	/* if the  */
	if (gtemp < n->mvcost) {
	  node->parent = n;
	  node->mvcost = node->parent->mvcost + trav(node->cord, node->parent->cord);
	  
	  walkable = (node_t **) realloc(walkable, sizeof(node_t *) + (++wcount));
	  walkable[wcount - 1] = node;
	  };
      } else {
	/* If it's untested, set the parent and 
	 * flag it as 'OPEN' for consideration */
	node->parent = n;
	node->mvcost = node->parent->mvcost + trav(node->cord, node->parent->cord);
	node->state = OPEN;

	walkable = (node_t **) realloc(walkable, sizeof(node_t *) * (++wcount));
	walkable[wcount - 1] = node;
      }
  }

  return (*count = wcount), walkable;
}

bool
seek(maze_t *m, node_t *c, uint xdim, uint ydim)
{

  /* set the current node to closed since
   * it cannot be traversed more than once */
  c->state = CLOSED;

  index i;
  uint ncount;
  
  /* list neighbors of the current node */
  node_t **neighbors = initneighbors(m, c, xdim, ydim, &ncount);

  puts("begin seek");
  for(i = 0; i < ncount; ++i)
    /* check whether the end node has been reached */
    if(neighbors[i]->cord->x == m->start->cord->x &&
       neighbors[i]->cord->y == m->target->cord->y) return true;
    /* if not, check the next set of nodes */
    else if(seek(m, neighbors[i], xdim, ydim)) return true;
  
  /* The method returns false if this path
   * leads to be a dead end */
  return false;
  
}

cord_t *
findpath(maze_t *m, cord_t *from, cord_t *to)
{
  puts("begin findpath");
  /* the start node is the first entry in the 'open' list */
  cord_t *path = NULL;
  uint pathsize = 0;
  node_t *start = m->map[from->x][from->y];
  
  if (seek(m, start, m->xdim, m->ydim)) {
    /* if a path was found, follow the parents 
     * from the end node to build a list of locations */

      puts("test seek");
    uint x = to->x, y = to->y;
    node_t *node = m->map[x][y];
      
    while(node->parent) {
      path = (cord_t *) realloc(path, sizeof(cord_t) * (++pathsize));
      path[pathsize - 1] = *(node->cord);
      node = node->parent;
    }

    /* reverse the list so it's in the correct
     * order when returned */
    /* path.Reverse(); */
  }

  path = (cord_t *) realloc(path, sizeof(cord_t) * (++pathsize));

  return path;
}



void
mfree(maze_t *m)
{
/*   index i, j; */
/*   /\* WHY THE FUCK WOULD SOMEONE PUT */
/*    * if(!p) {..} INSTEAD OF if(p)!!! *\/ */
/*   if(m) {     */
/* #ifdef MEM_DEBUG */
/*     printf("freeing.. "); */
/*     printf("m:\t %p\n----\n", m); */
/* #endif */
/*     for(i = 0; i < m->ydim; ++i) { */
/*       for(j = 0; j < m->ydim; ++j) { */
/* #ifdef MEM_DEBUG */
/* 	printf("freeing.. "); */
/* 	printf("m->map[%d][%d].cord:\t %p | .parent:\t %p\n",  */
/* 	       i, j, m->map[i][j].cord, m->map[i][j].parent); */
/* #endif */
/* 	free(m->map[i][j]->cord); */
/* 	free(m->map[i][j]->parent); */
/*       } */
/* #ifdef MEM_DEBUG */
/*       printf("\n----\n", m); */
/* #endif */
/*     } */
/*     for(i = 0; i < m->ydim; ++i) { */
/* #ifdef MEM_DEBUG */
/*       printf("freeing.. "); */
/*       printf("m->map[%d]:\t %p\n",i, m->map[i]); */
/* #endif */
/*       free(m->map[i]); */
/*     } */
/* #ifdef MEM_DEBUG */
/*     printf("freeing.. "); */
/*     printf("m->map:\t %p\n", m->map); */
/* #endif */
/*     free(m->map); */
/* #ifdef MEM_DEBUG */
/*     printf("freeing.. "); */
/*     printf("m->area:\t %p\n", m->area); */
/* #endif */
/*     free(m->area); */
/* #ifdef MEM_DEBUG */
/*     printf("freeing.. "); */
/*     printf("m:\t %p\n", m); */
/* #endif */
/*     free(m); */
/*   } */
}
