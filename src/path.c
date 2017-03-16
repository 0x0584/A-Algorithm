#include "../include/path.h"
#include "../include/lib.h"
#define DIRECTIONS 8

float
trav(cord_t *a, cord_t *b) {
    float deltaX = b->x - a->x;
    float deltaY = b->y - a->x;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
};

cord_t **
adjacents(cord_t *c, uint xdim, uint ydim)
{
  cord_t **foocord = (cord_t **) malloc(DIRECTIONS * sizeof(cord_t *));

  short directions[][2] = {{-1, -1}, {0, -1}, {0, -1},{1, 0},
		     {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};


  /* for(int i = 0; i < 8; ++i){ */
  /*   for(int j = 0; j < 2; ++j) */
  /*     printf("%d\t",directions[i][j]); */
  /*   putchar('\n'); */
  /* } */
  
  getchar();

  int i, nonpossible = 0;

  for(i = 0; i < DIRECTIONS; ++i) {
    uint x = c->x + directions[i][1],
      y = c->y + directions[i][0];
  
    /* Stay within the grid's boundaries */
    if(x < 0 || x >= (signed) xdim ||
       y < 0 || y >= (signed) ydim) {
	puts("not possible");
	printf("x:%d y: %d ",x, y);
	getchar();
	nonpossible++; continue;
    };

    foocord[i] = (cord_t *) malloc(sizeof(cord_t));
    
    foocord[i]->x = x;
    foocord[i]->y = y;

    printf("foocord[%d](%p):([(%d + (%d))]%d, [(%d + (%d))]%d)\n",
	   i, &foocord[i], c->x, directions[i][1], foocord[i]->x, c->y, directions[i][0], foocord[i]->y);
  };
  getchar();
  
  return foocord;
}

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count)
{
  node_t **walkable = NULL;
  index i, wcount = 0;		/* count of walkable nodes */

  /* get list of possible nodes */
  cord_t **next = adjacents(n->cord, xdim, ydim);
  walkable = (node_t **) malloc(sizeof(node_t *));
  printf("%p", walkable);
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
	};
      } else {
	/* If it's untested, set the parent and 
	 * flag it as 'OPEN' for consideration */
	node->parent = n;
	node->mvcost = node->parent->mvcost + trav(node->cord, node->parent->cord);
	node->state = OPEN;
	if(node->state == OPEN) printf("open: (%d, %d)", node->cord->x,node->cord->y);
      };
      
      walkable[wcount++] = (node_t *) malloc(sizeof(node_t ));
      walkable[wcount - 1] = node;

      printf("%d, %d: %d", walkable[wcount - 1]->cord->x, walkable[wcount - 1]->cord->y, walkable[wcount - 1]->state);
      getchar();
      putmaze(m);
      
      getchar();
  };

  puts("end init");
  return (*count = --wcount), walkable;
}

bool
seek(maze_t *m, node_t *c, uint xdim, uint ydim)
{

  /* set the current node to closed since
   * it cannot be traversed more than once */
  c->state = CLOSED;

  index i;
  static int u = 0;
  uint ncount;
  
  /* list neighbors of the current node */
  node_t **neighbors = initneighbors(m, c, xdim, ydim, &ncount);

  printf("begin seek%d \n", u++);

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

  printf("start(%d, %d)",start->cord->x, start->cord->y );
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


