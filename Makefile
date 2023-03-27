CC=gcc
CFLAGS=-I.
SRCDIR=src

server:
	$(CC) -o build/server src/*

clean:
	rm server