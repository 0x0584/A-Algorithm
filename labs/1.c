#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* maze dimensions */
#define XDIM 8
#define YDIM XDIM

enum AREA {START, TARGET};

typedef enum BOOL {
  false = (1==0),
  true = !(false)
} bool;

typedef struct COORDINATE {
  int x, y;
} cord_t;

typedef struct NODE {
  cord_t *cord;
  bool iswall;
  
} node_t;

char **map(unsigned int __xdim, unsigned int __ydim,
	   cord_t *__area);
void mfree(char **, unsigned int __ydim);

int
main()
{
  puts("$");
  srand(time(NULL));
  cord_t point[2];
  
  char **mmap = map(XDIM, YDIM, point);

  printf("START(%d, %d) -> TARGET(%d, %d)\n",
	 point[START].x, point[START].y,
	 point[TARGET].x, point[TARGET].y);

  for(unsigned i = 0; i < XDIM; ++i)
    puts(mmap[i]);
  
  mfree(mmap, YDIM);
  
  return 0;
}

char **
map(unsigned int x, unsigned int y, cord_t *a)
{
  char **m;		/* our map */

  /* allocate memory */
  m = (char **) malloc(x * sizeof(char *));
  
  for(unsigned int i = 0; i < x; ++i) {
    /* allocate memory */
    m[i] = malloc(y * sizeof(char));

    /* fill the map */
    for(unsigned int j = 0; j < y; ++j)
      if(!(rand()%5)) m[i][j] = '#';
      else m[i][j] = '.';
  }

  /* set a random starting-area / target-area */
  m[(a[START].x = rand()%x)][(a[START].y = rand()%y)] = 'S';
  m[(a[TARGET].x = rand()%x)][(a[TARGET].y = rand()%y)] = 'T';
  
  return m;
}

void
mfree(char **m, unsigned int y)
{
  if(!m) {
    for(unsigned int i = 0; i < y; ++i) free(m[i]);
    free(m);
  }
}
