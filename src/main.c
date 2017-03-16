/* [+] note to self:
 *
 * you vs time; who would be the winner? 
 * trust me, you don't really know what
 * would happen if you lose; an undefined 
 * behavior!
 */


#include "../include/types.h"
#include "../include/lib.h"

#include "../include/stack.h"

void
teststack()
{
  cord_t c = {0,0};
  node_t *head = initnode(&c);
  node_t *n = (node_t *) malloc(sizeof(node_t));

  push(&head, &c);
  push(&head, &c);
  push(&head, &c);
  push(&head, &c);
  push(&head, &c);

  for(node_t * foo = head; foo; foo = foo->next)
    printf("%d %d\n", foo->cord->x, foo->cord->y);
  
  if(pop(&head, n)) puts("ss");

    for(node_t * foo = head; foo; foo = foo->next)
    printf("%d %d\n", foo->cord->x, foo->cord->y);

  free(n);
  free(head);
}

/* the main function */
int
main()
{
  puts("$");
  teststack();
  getchar();
  srand(time(NULL));
  
  puts("#");
  maze_t *maze = initmaze();

/* #ifdef DEBUG   */
/*   printf("START(%d, %d) -> TARGET(%d, %d)\n", */
/* 	 maze->area[START].x, maze->area[START].y, */
/* 	 maze->area[TARGET].x, maze->area[TARGET].y); */
/* #endif */

  putmaze(maze);
  getchar();
  index i = 0;
  cord_t *path = findpath(maze, maze->start->cord, maze->target->cord);

  uint xtarget = maze->target->cord->x,
    ytarget = maze->target->cord->y;
  
  while(path[i].x != xtarget && path[i].y != ytarget) {
    uint x = path[i].x, y = path[i].y;
    maze->map[x][y]->state = IN_PATH;
    ++i;
  }
  
  putmaze(maze);
  mfree(maze);
  
  return 0;
}
