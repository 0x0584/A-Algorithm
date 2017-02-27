#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readfile(char *path);
char *readf(FILE *file);

char *readinput( void );

int
main(int argc, char **argv)
{
  char *maze = (argc < 2) ? readinput() :readfile(argv[1]);

  puts(maze);
  
  return EXIT_SUCCESS;
}

char *
readfile(char *path)
{
  FILE *foo = NULL;
  char *content = NULL;

  /* getting the content of the file (in `path`)  */
  if(!(content = readf(foo = fopen(path, "r"))))
    fprintf(stderr, "cannot open the file (%s).", path), exit(EXIT_FAILURE);

  /* always close the file-pointer */
  fclose(foo);
  
  return content;
}

char *
readf (FILE *f)
{

  int string_size = 0, read_size = -1;
  char *buffer = NULL;

  /* the file-pointer IS NOT NULL */
  if(f) {
    fseek(f, 0, SEEK_END); /* Beginning of the stream */
    string_size = ftell(f); /* Size of the stream */
    rewind(f);

    /* allocate memory  */
    buffer = (char*) malloc((string_size + 1) * sizeof(char));
        
    /* Binary size of the stream */
    read_size = fread(buffer, sizeof(char), string_size, f);
    buffer[string_size] = '\0';

    /* /\* The readied size is != to stream size *\/ */
    /* if (string_size != read_size)  */
    /*   free(buffer), buffer = NULL; */
  
  }

  /* return buffer; */

  /* readied size != stream size) 
   * this implies that there was an error while 
   * reading the file, return NULL. 
   * otherwise, return the buffer */
  return (string_size != read_size) ? free(buffer), buffer = NULL: buffer;
}

char *
readinput( void )
{
}
