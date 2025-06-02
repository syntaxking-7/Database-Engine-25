CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: btree

btree: main.o btree.o
	$(CC) $(CFLAGS) main.o btree.o -o btree

main.o: main.c btree.h
	$(CC) $(CFLAGS) -c main.c

btree.o: btree.c btree.h
	$(CC) $(CFLAGS) -c btree.c

clean:
	rm -f *.o btree
