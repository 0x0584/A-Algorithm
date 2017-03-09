#ifndef __LIB_H
#define __LIB_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define FVALUE(x, y) (x + y)

#define WALLS 2

/* functions protoype */
maze_t *initmaze(void);
node_t **initmap(uint __xdim, uint __ydim, /* map dimensions */
		 cord_t *__area);	   /* set of points */
void putmaze(maze_t *__maze);
void mfree(maze_t *__maze);
void seek(maze_t *__maze, cord_t *__area);

#endif	/* __LIB_H */