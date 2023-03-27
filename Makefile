CC=gcc
CFLAGS=-I.
SRCDIR=src

server:
	$(CC) -o server src/*

clean:
	rm server