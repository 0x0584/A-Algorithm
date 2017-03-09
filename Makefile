# AUTHOR: ANAS RCHID
# LAST UPDATE: 03/09/2017 09:26:25

CC = gcc
CFLAGS = -ggdb -O2 -Wall -Wextra -Wpadded \
	-D DEBUG # -pedantic -D MEM_DEBUG #-save-temps
LDFLAGS = -I. #-lncurses

EXEC = maze
ARGS = #

SRCDIR = src
OBJDIR = bin
DEPSDIR = include

SRC := $(shell find $(SRCDIR) -name '*.c')
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DEPS:= $(shell find $(DEPSDIR) -name '*.h')

RM = rm -f
DRM = rm -fr

all: main
	@echo "finish."

main: build-dir $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)


build-dir:
	@$(call make-dir)

define make-dir
	for dir in $(OBJDIR); \
	do \
	mkdir -p $$dir; \
	done
endef

run: remake
	@clear
	./$(EXEC) $(ARGS)

remake: clean all

clean:
	$(RM) $(OBJ) $(EXEC)
