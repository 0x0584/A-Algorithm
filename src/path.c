#include "../include/path.h"
#include "../include/lib.h"
#define DIRECTIONS 8

float
trav(cord_t *a, cord_t *b) {
  printf("(%d, %d) | (%d, %d) ", a->x, a->y, b->x, b->y);
  
  int deltaX = (int) b->x - (int) a->x;
  int deltaY = (int) b->y - (int) a->y;

    printf("%d-%d=%d | %d-%d=%d: %f\n-----\n",
	   b->x, a->x, deltaX,
	   b->y, a->y, deltaY,
	   sqrt(deltaX * deltaX + deltaY * deltaY));
    return sqrt(deltaX * deltaX + deltaY * deltaY);
};

cord_t **
adjacents(cord_t *c, uint xdim, uint ydim, uint *count)
{
  cord_t **foocord = (cord_t **) malloc(DIRECTIONS * sizeof(cord_t *));

  short directions[][2] = {
    {-1,  0},			/* up */
    {+1,  0},			/* down */
    { 0, -1},			/* left */
    { 0, +1},			/* right */
    {-1, -1},			/* upper left */
    {-1, +1},			/* upper right */
    {+1, +1},			/* bottom right */
    {+1, -1}			/* bottom left */
  };

  /* for(int i = 0; i < 8; ++i){ */
  /*   for(int j = 0; j < 2; ++j) */
  /*     printf("%d\t",directions[i][j]); */
  /*   putchar('\n'); */
  /* } */
  
  getchar();

  int i, j = 0, nonpossible = 0;

  for(i = 0; i < DIRECTIONS; ++i) {
    int x = c->x + directions[i][0],
      y = c->y + directions[i][1];
  
    /* Stay within the grid's boundaries */
    if(x < 0 || x >= (signed) xdim ||
       y < 0 || y >= (signed) ydim) {
	puts("not possible");
	printf("x:%d y: %d ",x, y);
	getchar();
	nonpossible++; continue;
    };

    foocord[j] = (cord_t *) malloc(sizeof(cord_t));
    
    foocord[j]->x = x;
    foocord[j]->y = y;

    printf("foocord[%d](%p):([(%d + (%d))]%d, [(%d + (%d))]%d)\n",
	   j, &foocord[j],
	   c->x, directions[i][1], foocord[j]->x,
	   c->y, directions[j][0], foocord[j]->y);
    ++j;
  };
  getchar();

  *count = j;
  puts("end adj");
  return foocord;
}

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count)
{
  node_t **walkable = (node_t **) malloc(sizeof(node_t *));

  index i, j;
  uint wcount = 0, adcount;		/* count of walkable nodes */

  /* get list of possible nodes */
  cord_t **next = adjacents(n->cord, xdim, ydim, &adcount);

  for(i = 0; i < adcount; ++i)
    printf("(%d, %d) G:%u H:%f F:%f\n",next[i]->x, next[i]->y,
	   m->map[next[i]->x][next[i]->y]->mvcost,
	   m->map[next[i]->x][next[i]->y]->heuristic,
	   FVALUE(m->map[next[i]->x][next[i]->y]->mvcost,
		  m->map[next[i]->x][next[i]->y]->heuristic ));
  
  for(j = 1; j < adcount; ++j) 
    for(i = 0; i < adcount - 1; ++i){
      uint g1 = m->map[next[i]->x][next[i]->y]->mvcost,
	g2 = m->map[next[i+1]->x][next[i+1]->y]->mvcost;
      float h1 = m->map[next[i]->x][next[i]->y]->heuristic,
	h2 = m->map[next[i+1]->x][next[i+1]->y]->heuristic;


      if((g1 + h1) < (g2 + h2)) {
	printf("%d, %d | %d, %d\t", next[i]->x, next[i]->y, next[i+1]->x, next[i+1]->y);
	cord_t *foo = next[i];
	next[i] = next[i+1];
	next[i+1] = foo;
	printf("%d, %d | %d, %d\n", next[i]->x, next[i]->y, next[i+1]->x, next[i+1]->y);
      } 
    }

  puts("after");
  for(i = 0; i < adcount; ++i)
    printf("(%d, %d) G:%u H:%f F:%f\n",next[i]->x, next[i]->y,
	   m->map[next[i]->x][next[i]->y]->mvcost,
	   m->map[next[i]->x][next[i]->y]->heuristic,
	   FVALUE(m->map[next[i]->x][next[i]->y]->mvcost,
		  m->map[next[i]->x][next[i]->y]->heuristic ));
  getchar();
  printf("walkable: %p\n", walkable);

  puts("initneighbors");

  for(i = 0; i < DIRECTIONS; ++i) {

    printf("loop:%d", i);    
    uint x = next[i]->x;
    uint y = next[i]->y;

    printf("(%s)m->map[%d][%d] = (%d, %d)\n", 
	   m->map[x][y]->iswall ? "wall" : "nowall" , x, y, 
	   m->map[x][y]->cord->x, m->map[x][y]->cord->y);
      
    node_t *node = m->map[x][y];
      
    /* ignore non-walkable nodes */
    if(node->iswall) continue;

    /* ignore already-closed nodes */
    if(node->state == CLOSED) {
      printf("closed:(%d, %d)", node->cord->x,node->cord->y);
      continue;
    }

    if(node->state == OPEN)
      printf("open: (%d, %d)", node->cord->x,node->cord->y);
      
    /* Already-open nodes are only added to the
     * list if their G-value is lower going via
     * this route. */
    if(node->state == OPEN) {
      float traversalcost = trav(node->cord, node->parent->cord);
      float gtemp = n->mvcost + traversalcost;

      /* if the  */
      if(gtemp > n->mvcost) {
	node->parent = n;
	node->mvcost = node->parent->mvcost + trav(node->cord, node->parent->cord);
	printf("%p, %d", node->parent, node->mvcost);
      };
    } else {
      /* If it's untested, set the parent and 
       * flag it as 'OPEN' for consideration */
      node->parent = n;
      node->mvcost = node->parent->mvcost + trav(node->cord, node->parent->cord);
      node->state = OPEN;
    };
      
    walkable[wcount++] = (node_t *) malloc(sizeof(node_t ));
    walkable[wcount - 1] = node;

    printf("walkable[%d]%d, %d: %d\n", wcount -1,
	   walkable[wcount - 1]->cord->x,
	   walkable[wcount - 1]->cord->y,
	   walkable[wcount - 1]->state);

    getchar();
    putmaze(m);
      
    getchar();
  };

  puts("end loop");
  *count = --wcount;
  printf("%d\n", *count);

  for(i = 0; i < *count; ++i)
    printf("walkable[%d]%d, %d: %d\n", i,
	   walkable[i]->cord->x,
	   walkable[i]->cord->y,
	   walkable[i]->state);

  getchar();

  puts("#");
  for(uint i = 1; i < *count; ++i)
    for(uint j = 0; j < *count - 1; ++j) {
      float h1 = walkable[j]->heuristic, h2 = walkable[j+1]->heuristic;
      uint g1 = walkable[j]->mvcost, g2 = walkable[j+1]->mvcost;
      
      if((h1 + g1) > (h2 + g2)) {
	printf("%d, %d | %d, %d\t", walkable[j]->cord->x, walkable[j]->cord->y, walkable[j+1]->cord->x, walkable[j+1]->cord->y);
	node_t *foo = walkable[j];
	walkable[j] = walkable[j+1];
	walkable[j+1] = foo;
	printf("%d, %d | %d, %d\n", walkable[j]->cord->x, walkable[j]->cord->y, walkable[j+1]->cord->x, walkable[j+1]->cord->y);
      }
    }
    return walkable;
}

bool
seek(maze_t *m, node_t *c, uint xdim, uint ydim)
{

  /* set the current node to closed since
   * it cannot be traversed more than once */
  c->state = CLOSED;

  index i;
  static int u = 0;
  uint ncount;			/* neighbors count */
  
  /* list neighbors of the current node */
  node_t **neighbors = initneighbors(m, c, xdim, ydim, &ncount);

  printf("begin seek%d \n", u++);

    for(i = 0; i < ncount; ++i) {
      printf("neighbor[%d]%d, %d: %d : %d\n", i,
	   neighbors[i]->cord->x,
	   neighbors[i]->cord->y,
	     neighbors[i]->state,
	     neighbors[i]->mvcost + neighbors[i]->heuristic);

    }
    getchar();
  for(i = 0; i < ncount; ++i) {
    /* check whether the end node has been reached */
    if(neighbors[i]->cord->x == m->target->cord->x &&
       neighbors[i]->cord->y == m->target->cord->y) return true;
    /* if not, check the next set of nodes */
    else if(seek(m, neighbors[i], xdim, ydim)) return true;
  }
  
  /* The method returns false if this path
   * leads to be a dead end */
  return false;
}

cord_t **
findpath(maze_t *m, cord_t *from, cord_t *to)
{
  puts("begin findpath");
  /* the start node is the first entry in the 'open' list */
  cord_t **path = (cord_t **) malloc(sizeof(cord_t *));
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
      path[pathsize++] = (cord_t *) malloc(sizeof(cord_t));
      
      node->parent->state = IN_PATH;
      path[pathsize - 1] = node->cord;
      node = node->parent;
    }

    /* reverse the list so it's in the correct
     * order when returned */
    /* path.Reverse(); */
  }

  return path;
}


