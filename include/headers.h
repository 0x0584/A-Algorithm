#ifndef __HEADERS_H
#define __HEADERS_H


#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <math.h>

#include "../include/types.h"

bool seek(maze_t *__maze, node_t *__current,
	  uint __maze_xdim, uint __maze_ydim);
cord_t *
findpath(maze_t *m, cord_t *from, cord_t *to);

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count);
cord_t **
adjacents(cord_t *c, uint xdim, uint ydim);


#endif	/* __HEADERS_H */
