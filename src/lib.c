
#include "../include/lib.h"
#include "../include/stack.h"
#include "../include/path.h"

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
      m[i][j] = (node_t *) malloc(sizeof(node_t ));
      printf("m[%d][%d]: %p\n", i, j, m[i][j]);
      
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
  getchar();
    puts("end initmap");
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
	if(state == OPEN) putcolored((nodeval = '*'), FRED, BBLACK); 
	else if(state == CLOSED) putcolored((nodeval = '$'), FBLUE, BRED);
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
