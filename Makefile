# Makefile
CPPFLAGS = `pkg-config --cflags sdl2` -MMD
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -std=c99 -O2 -g -w
#LDFLAGS = -fsanitize=address
LDLIBS =` pkg-config --libs sdl2 gtk+-3.0` -lSDL2 -lSDL2_image -lm


SRC = main.c glade/interface.c encode/encode.c encode/data_encoding/encode_string.c encode/data_encoding/polynomial.c encode/data_encoding/final_struct.c encode/data_encoding/makeqrcode.c encode/data_encoding/qrmask.c decode/patterns.c decode/decode.c decode/colors.c pre-processing/load.c pre-processing/img_operation.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ} 

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program
	${RM} out.bmp

# END

