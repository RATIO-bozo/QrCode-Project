# Makefile
CC = gcc
CFLAGS =-Wall -Wextra -std=c99 -O2 -g

SRC = test.c encode_string.c polynomial.c makeqrcode.c final_struct.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: test 

test: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} test     # remove main program

# END
