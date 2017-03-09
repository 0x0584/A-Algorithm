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

/* the main function */
int
main()
{
  puts("$");
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
