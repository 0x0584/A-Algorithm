#include <math.h>

#include "../include/lib.h"
#include "../include/stack.h"

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

      m[i][j].isopen = true; 
      m[i][j].ismarked = m[i][j].belongstopath = false;
    }
  }

  /* set random coordinates for the starting-area */
  a[START].x = rand()%x;
  a[START].y = rand()%y;
  
  /* set random coordinates for the target-area */
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
      bool wall = m->map[i][j].iswall, marked = m->map[i][j].ismarked,
	belongstopath = m->map[i][j].belongstopath;
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
      case STARTCHAR: putcolored(nodeval, FBLACK, BYELLOW); break;
      case TARGETCHAR: putcolored(nodeval, FBLACK, BRED); break;

      default:
	if(!marked) putcolored(nodeval, FBLACK, BWHITE);
	else if(belongstopath) putcolored((nodeval = 'o'), FRED, BGREEN);
	else putcolored((nodeval = '*'), FRED, BCYAN);
	  
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

bool
seek(maze_t *m, cord_t *a)
{
  puts("staring seek..");
#define POSSIBLE_MOVES 8 /* no limit on the moves */

#define UP(w, x, y)		w[x - 1][y + 0]
#define DOWN(w, x, y)		w[x + 1][y + 0]
#define LEFT(w, x, y)		w[x + 0][y - 1]
#define RIGHT(w, x, y)		w[x + 0][y + 1]
#define UPLEFT(w, x, y)	w[x - 1][y - 1]
#define DOWNLEFT(w, x, y)	w[x + 1][y - 1]
#define UPRIGHT(w, x, y)	w[x - 1][y + 1]
#define DOWNRIGHT(w, x, y)	w[x + 1][y + 1]

#define UPCORD(c)		((c.x - 1) && (c.y + 0))
#define DOWNCORD(c)		((c.x + 1) && (c.y + 0))
#define LEFTCORD(c)		((c.x + 0) && (c.y - 1))
#define RIGHTCORD(c)		((c.x + 0) && (c.y + 1))
#define UPLEFTCORD(c)		((c.x - 1) && (c.y - 1))
#define DOWNLEFTCORD(c)	((c.x + 1) && (c.y - 1))
#define UPRIGHTCORD(c)	((c.x - 1) && (c.y + 1))
#define DOWNRIGHTCORD(c)	((c.x + 1) && (c.y + 1))  

  /* local functions */  
  void insert(node_t **head, cord_t *cord, uint mvcost) {
    uint xtar = a[TARGET].x, ytar = a[TARGET].y, 
      xcur = cord->x, ycur = cord->y;
    node_t *new = initnode(cord);
    node_t *one, *two;
    float distance = sqrt(pow((xtar - xcur), 2) + pow((ytar - ycur), 2));
    
    new->mvcost = mvcost;
    new->heuristic = distance;

    if(isempty(head)) (*head) = new, (*head)->next = NULL; 
    else {
      one = (*head), two = NULL;

      /*  */
      while(one) {
	if((one->heuristic + one->mvcost) > distance + mvcost) break;
	two = one, one = one->next;
      }

      /*  */
      new->next = one;
      if(!two) (*head) = new;
      else two->next = new;
    }
  };

  void successor(node_t **head, cord_t cord, uint mvcost) {

    uint x = cord.x, y = cord.y;

    if(x != a[TARGET].x && y != a[TARGET].y) {
      if(DOWN(m->map, x, y).isopen && !DOWN(m->map, x, y).iswall &&
	 DOWNCORD(cord)) {
	DOWN(m->map, x, y).isopen = false;
	DOWN(m->map, x, y).ismarked = true;
	insert(head, DOWN(m->map, x, y).cord, mvcost);
      }

      if(UP(m->map, x, y).isopen && !UP(m->map, x, y).iswall &&
	 UPCORD(cord)) {
	UP(m->map, x, y).isopen = false;
	UP(m->map, x, y).ismarked = true;
	insert(head, UP(m->map, x, y).cord, mvcost);
      }

      if(LEFT(m->map, x, y).isopen && !LEFT(m->map, x, y).iswall &&
	 LEFTCORD(cord)) {
	LEFT(m->map, x, y).isopen = false;
	LEFT(m->map, x, y).ismarked = true;
	insert(head, LEFT(m->map, x, y).cord, mvcost);
      }

      if(RIGHT(m->map, x, y).isopen && !RIGHT(m->map, x, y).iswall &&
	 RIGHTCORD(cord)) {
	RIGHT(m->map, x, y).isopen = false;
	RIGHT(m->map, x, y).ismarked = true;
	insert(head,  RIGHT(m->map, x, y).cord, mvcost);
      }
    
      if(DOWNLEFT(m->map, x, y).isopen && !DOWNLEFT(m->map, x, y).iswall &&
	 DOWNLEFTCORD(cord)) {
	DOWNLEFT(m->map, x, y).isopen = false;
	DOWNLEFT(m->map, x, y).ismarked = true;
	insert(head, DOWNLEFT(m->map, x, y).cord, mvcost);
      }
    
      if(DOWNRIGHT(m->map, x, y).isopen && !DOWNRIGHT(m->map, x, y).iswall &&
	 DOWNRIGHTCORD(cord)) {
	DOWNRIGHT(m->map, x, y).isopen = false;
	DOWNRIGHT(m->map, x, y).ismarked = true;
	insert(head, DOWNRIGHT(m->map, x, y).cord, mvcost);
      }
    
      if(UPLEFT(m->map, x, y).isopen && !UPLEFT(m->map, x, y).iswall &&
	 UPLEFTCORD(cord)) {
	UPLEFT(m->map, x, y).isopen = false;
	UPLEFT(m->map, x, y).ismarked = true;
	insert(head, UPLEFT(m->map, x, y).cord, mvcost);
      }
    
      if(UPRIGHT(m->map, x, y).isopen && !UPRIGHT(m->map, x, y).iswall &&
	 UPRIGHTCORD(cord)) {
	UPRIGHT(m->map, x, y).isopen = false;
	UPRIGHT(m->map, x, y).ismarked = true;
	insert(head, UPRIGHT(m->map, x, y).cord, mvcost);
      }
    } else puts("IS TARGET!!");
  };

  /* local variables */
  uint mvcost = 0;		    /* # of nodes in the maze */
  uint xtar = a[TARGET].x, ytar = a[TARGET].y;
  node_t *head = initnode(&a[START]), *data;		    /* data is to get the poped data */


  /* function body */
  /* starting.. */
#ifdef DEBUG
  printf("head: %p - START(%d %d) - cost: %d\t", &head, a[START].x, a[START].y, mvcost);
#endif

  data = (node_t *) malloc(sizeof(node_t));

  m->map[a[START].x][a[START].y].isopen = false;
  
  /* add te starting area to our linkedlist */
  insert(&head, &a[START], mvcost);
  
  int i = 0;

  /* seeking the target.. */
  while(true) {
    cord_t current;

    /* if there is no left node, no solution */
    if(isempty(&head)) return false; /* no solution */

    /* pop current node into data */

    pop(&head, data);
    current = *(data->cord);

    m->map[current.x][current.y].belongstopath = true;

#ifdef DEBUG   
    printf("head: %p - current(%d %d) - cost: %d\t", &head, current.x, current.y, mvcost);
#endif
    
    /* set current to false */
    m->map[current.x][current.y].isopen = false;
    
    if(current.x == xtar && current.y == ytar)
      return true;
    /* getchar(); */

    /* system("clear"); */

    successor(&head, current, ++mvcost);

    putmaze(m);
    getchar();
#ifdef DEBUG
    printf("%d time(s)\n", i++);
#endif

  }

  free(data);
  free(head);
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
