#ifndef __TYPES_H
#define __TYPES_H

typedef enum Boolean bool;

typedef unsigned int index;
typedef unsigned int uint;
typedef struct COORDINATE cord_t;
typedef struct NODE node_t;
typedef struct MAZE maze_t;

enum Boolean {
  false = (1==0),
  true = !(false)
};

struct COORDINATE {
  uint x; 			/* x-axis */
  uint y;			/* y-axis */
};

struct NODE {
  enum NODESTATE{
    UNCHECKED,
    OPEN, CLOSED,
    IN_PATH,
    START = -1, TARGET = -1
  } state;
  
  cord_t *cord;			/* coordinate of the node */
  node_t *parent;		/* parent of the node */

  bool iswall;			/* is wall: true | false */
  uint mvcost;			/* G value is vary between 
				 * horizontal/vertical directions*/
  float heuristic;		/* H value */

  node_t *next;		/* next node */
  /* for performance reasons; avoiding 
   * decimals! using a whole number is 
   * a lot faster.. */
  #define G_HV 10		/* this is actually 1 */
  #define G_DIAG 14		/* this is actually sqrt(2) */
};

struct MAZE {
  node_t ***map;		/* maze map */
  uint xdim, ydim;		/* map dimensions */

  /* starting and target area */
  node_t *start, *target;
};

#endif	/* __TYPES_H */
