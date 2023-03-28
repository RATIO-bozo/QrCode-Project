# Makefile
CC = gcc
CPPFLAGS=`sdl2-config --cflags`
CFLAGS = -Wall -Wextra -std=c99 -O2 -g 
LDLIBS = `sdl2-config --libs`


SRC = encode.c data_encoding/encode_string.c data_encoding/polynomial.c data_encoding/final_struct.c data_encoding/makeqrcode.c data_encoding/qrmask.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: encode

encode: ${OBJ} 

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} encode     # remove main program
	${RM} out.bmp

# END

