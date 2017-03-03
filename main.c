#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum BOOL bool;
typedef struct COORDINATE cord_t;
typedef struct DIREC direc_t;
typedef struct MAZE maze_t;

enum BOOL{
  false = (1 == 0),
  true = !false
};

enum DIRECTION {
  NORTH = 0, SOUTH,
  EAST, WEST
};

struct COORDINATE {
  int x, y;
};

struct DIREC {
  short direc:2;
};

struct MAZE {
  char *map;			/* the maze map */
  cord_t mdim;			/* maze dimension */
  cord_t *start, *target;	/* Starting area and Target area */
  
  bool issolvable;		/* is the maze solvable */
  direc_t *path;		/* list of direction to go from S to T */
  
};

char *readfile(char *path);
char *readinput();
maze_t *minit(int, char **);
char *parsemaze(char *);
void mfree(maze_t *);
cord_t *getcord(char *maze, short mod);

int
main(int argc, char **argv)
{

  puts("$");
  maze_t *maze =  minit(argc, argv);

  /* testing.. */
  puts(maze->map);

  /* don't forget to free the maze! */
  mfree(maze);
 
  return EXIT_SUCCESS;
}

maze_t *
minit(int c, char **v)
{
  puts("$");
  enum {START, TARGET};

  maze_t *foomaze = NULL;
  
  if(!(foomaze = (maze_t *) calloc(1, sizeof(maze_t))))
    goto RET;
  
  if(!(foomaze->map = parsemaze((c < 2) ? readinput() : readfile(v[1]))) ||
     !(foomaze->start = getcord(foomaze->map, START)) ||
     !(foomaze->target = getcord(foomaze->map, TARGET))) {
    mfree(foomaze);
    return NULL;
  }

  
 RET:
  return foomaze;
}

char *
parsemaze(char *file)
{
  unsigned int i;
  char *maze, *str,
    possible[] = {'#', '.'};

  maze = (char *) malloc(strlen(file));

  /* find the begining of the maze */
  for(i = 0; i < sizeof(possible)/sizeof(possible[0]); ++i)
    if(!(maze = strchr(file, possible[i]))) continue;
    else break;

  /* find the end of the maze */
  for(i = 0; i < sizeof(possible)/sizeof(possible[0]); ++i)
    if(!(str = strrchr(file, possible[i]))) continue;
    else break;

  maze[maze - str] = '\0';

  puts(maze);
  return maze;
}

cord_t *
getcord(char *maze, short mod)
{
  cord_t *foocord = NULL;
  unsigned int i;
  char *newline = strchr(maze, '\n'),
    target[] = { 'S','T'};

  if(!(foocord = malloc(sizeof(cord_t))))
    goto RET;

  /* the X coordinate is the distination from 
   * the beginning MOD the length of one line */
  foocord->x = (maze - strchr(maze, target[mod])) % (maze - newline);

    puts("$");  
    printf("%ld", (maze - strchr(maze, target[mod])) % (maze - newline));
  /* the Y coordinate is # number of '\n' we 
   * have passed throw */
  for(i = 0; i < strlen(maze);) {
    /* count the number of lines */
    if(maze[i] == '\n') ++i;
    if(maze[i] == target[mod])
      foocord->y = i;
  puts("#");
  }
 RET:
  return foocord;
}

char *
readfile(char *path)
{
  /* local prototypes */
  char *readf(FILE *);
  
  FILE *foo = NULL;
  char *content = NULL;

  /* getting the content of the file (in `path`)  */
  if(!(content = readf(foo = fopen(path, "r"))))
    fprintf(stderr, "cannot open the file (%s).", path);

  /* always close the file-pointer */
  fclose(foo);
  
  return content;
}

char *
readf (FILE *f)
{

  int string_size = 0, read_size = -1;
  char *buffer = NULL;

  if(!f) goto RET; /* the file-pointer is NULL */

  fseek(f, 0, SEEK_END); /* Beginning of the stream */
  string_size = ftell(f); /* Size of the stream */
  rewind(f);

  /* allocate memory  */
  buffer = (char*) malloc((string_size + 1) * sizeof(char));
        
  /* Binary size of the stream */
  read_size = fread(buffer, sizeof(char), string_size, f);
  buffer[string_size] = '\0';
  
  /* readied size != stream size) 
   * this implies that there was an error while 
   * reading the file, return NULL. 
   * otherwise, return the buffer */
 RET:
  return (string_size != read_size) ? free(buffer), buffer = NULL: buffer;
}

char *
readinput( void )
{
  return "NONE";
}

void
mfree(maze_t *m)
{
  if(m) {
    free(m->start);
    free(m->target);
    free(m->map);
    free(m->path);
    free(m);
  }
}
