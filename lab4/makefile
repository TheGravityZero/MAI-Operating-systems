CC = gcc
CFLAGS = -c -Wall -g
FINALFLAGS = -pthread  -o 

all: parent child

parent: parent.o
	$(CC) parent.o $(FINALFLAGS) parent -lrt
parent.o: parent.c
	$(CC) $(CFLAGS) parent.c
child: child.o
	$(CC) child.o $(FINALFLAGS) child -lrt
child.o: child.c
	$(CC) $(CFLAGS) child.c
clean:
	rm -rf parent child child.o