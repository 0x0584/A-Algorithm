#ifndef __LIB_H
#define __LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"

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

#define FVALUE(G, H) (G + H)

#define WALLS 3

/* functions protoype */
maze_t *initmaze(void);
node_t ***initmap(uint __xdim, uint __ydim, node_t *tonode);
void putmaze(maze_t *__maze);
void mfree(maze_t *__maze);
bool seek(maze_t *__maze, node_t *__current,
	  uint __maze_xdim, uint __maze_ydim);
cord_t *
findpath(maze_t *m, cord_t *from, cord_t *to);

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count);
cord_t **
adjacents(cord_t *c, uint xdim, uint ydim);



#endif	/* __LIB_H */
