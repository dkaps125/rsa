CC = gcc
CFLAGS = -lgmp
DEBUG = -g

all: rsa.o
	$(CC) $(CFLAGS) -o rsa rsa.o

debug:
	$(CC) $(CFLAGS) -o rsa -g rsa.o

clean:
	rm rsa *.o *~ 2> /dev/null

rsa.o: rsa.c rsa.h
	$(CC) $(CFLAGS) -c rsa.c
