CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -w -O3 `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lm

all: cut urlEncoder

SRC = cut.c urlEncoder.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

exec: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
