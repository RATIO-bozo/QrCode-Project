CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -w  `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lm

all: QrCodeAnalyser

SRC = QrCodeAnalyser.c 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

exec: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
