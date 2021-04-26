CC = gcc
CFLAGS = -g3 -c -Wall

all: prog1 prog2

prog1: lib1.so 
	$(CC) main_static.c -L. -l1  -Wall -o main_static

prog2: lib1.so lib2.so
	$(CC) main_dynamic.c  -o  main_dynamic -ldl

lib1.so: lib1.o
	$(CC) lib1.o  -shared -o lib1.so -lm

lib2.so: lib2.o
	$(CC) lib2.o  -shared -o lib2.so -lm

lib1.o:
	$(CC) $(CFLAGS) functions_01.c -lm -Werror -fpic  -o lib1.o

lib2.o:
	$(CC) $(CFLAGS) functions_02.c -lm -Werror -fpic -o lib2.o

clean:
	rm -rf *.so *.o main_dynanmic main_static
