# Makefile
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -std=c99 -O2 -g
LDFLAGS =
LDLIBS =` pkg-config --libs sdl2 gtk+-3.0` -lSDL2 -lSDL2_image -lm

SRC= test.c load.c img_operation.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: test

test: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program
# END

