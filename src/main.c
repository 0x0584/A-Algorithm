/* [+] note to self:
 *
 * you vs time; who would be the winner? 
 * trust me, you don't really what would happen 
 * if you lose; an undefined behavior!
 *
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

  node_t *temp;

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
  
  maze_t *maze = initmaze();

#ifdef DEBUG  
  printf("START(%d, %d) -> TARGET(%d, %d)\n",
	 maze->area[START].x, maze->area[START].y,
	 maze->area[TARGET].x, maze->area[TARGET].y);
#endif

  putmaze(maze);

  seek(maze, maze->area);
  
  mfree(maze);
  
  return 0;
}
