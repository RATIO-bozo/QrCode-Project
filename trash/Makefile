CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image ` -lm -lX11 -lpng
 
all: cut urlEncoder qr2 sout

SRC = cut.c urlEncoder.c qr2.c sout.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
