#ifndef __PATH_H
#define __PATH_H

#include "../include/types.h"
#include "../include/headers.h"

bool seek(maze_t *__maze, node_t *__current,
	  uint __maze_xdim, uint __maze_ydim);
cord_t *
findpath(maze_t *m, cord_t *from, cord_t *to);

node_t **
initneighbors(maze_t *m, node_t *n, uint xdim, uint ydim, uint *count);
cord_t **
adjacents(cord_t *c, uint xdim, uint ydim);
float trav(cord_t *a, cord_t *b) ;


#endif	/* __PATH_H */
