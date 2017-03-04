CC = gcc
CFLAGS = -Wall #-Werror -Wextra -O2
LDFLAGS = #-I. -lncurses

RM = rm -f

OBJ = main.o 
DEPS = #

EXEC = maze
ARGS = #

all: main
	@echo "finish."

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

run: remake
	./$(EXEC) $(ARGS)

remake: clean all

clean:
	$(RM) $(OBJ) $(EXEC)
