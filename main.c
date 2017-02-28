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
  char *map;
  cord_t start, finish;
  
  bool issolvable;
  direc_t *path;
  
} *maze;

char *readfile(char *path);
char *readf(FILE *file);
char *readinput( void );
void mfree(maze_t *);

int
main(int argc, char **argv)
{

  char *mmap = (argc < 2) ? readinput() : readfile(argv[1]);

  puts("#");

  maze = (maze_t *) calloc(1, sizeof(maze_t));

  puts((maze->map = mmap));

  mfree(maze);
  
  return EXIT_SUCCESS;
}

char *
readfile(char *path)
{
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
    free(m->map);
    free(m->path);
    free(m);
  }
}
