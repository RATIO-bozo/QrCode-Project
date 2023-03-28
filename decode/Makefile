#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
#LDFLAGS = -fsanitize=address -lm
LDFLAGS = -lm
LDLIBS =` pkg-config --libs sdl2 gtk+-3.0` -lSDL2 -lSDL2_image -lm

SRC = patterns.c decode.c colors.c ../pre-processing/img_operation.c ../pre-processing/load.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: decode

decode: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} # remove .o files
	${RM} ${DEP} # remove .d files
	${RM} decode # remove executable

# END


