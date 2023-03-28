all : test

test: interface.o
	gcc -Wall interface.o -o test `pkg-config --libs gtk+-3.0` `pkg-config --cflags gtk+-3.0`


interface.o: interface.c
	gcc -c -Wall interface.c -o interface.o `pkg-config --libs gtk+-3.0` `pkg-config --cflags gtk+-3.0`

clean:
	rm -rf *.o test
