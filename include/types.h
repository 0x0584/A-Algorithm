#ifndef __TYPES_H
#define __TYPES_H

typedef enum BOOL {
  false = (1==0),
  true = !(false)
} bool;

typedef unsigned int index;
typedef unsigned int uint;

typedef struct COORDINATE {
  uint x; 			/* x-axis */
  uint y;			/* y-axis */
} cord_t;

typedef struct NODE {
  cord_t *cord,			/* coordinate of the node */
    *parent;			/* parent of the node */
  bool iswall;			/* is wall: true | false */
  uint mvcost,			/* G value is vary between 
				 * horizontal/vertical directions*/
    heuristic;			/* H value */
  bool isopen;  		/* if true, we can take consider
				 * this node as a future pathway */
  struct NODE *next;		/* next node */
  /* for performance reasons; avoiding 
   * decimals! using a whole number is 
   * a lot faster.. */
  #define G_HV 10		/* this is actually 1 */
  #define G_DIAG 14		/* this is actually sqrt(2) */
} node_t;

typedef struct MAZE {
  node_t **map;			/* maze map */
  uint xdim, ydim;		/* map dimensions */

  cord_t *area;			/* starting and target area */
} maze_t;

#endif	/* __TYPES_H */