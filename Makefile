CC=gcc
CFLAGS=-I.
SRCDIR=src

server:
	$(CC) -o build/server src/*

client:
	$(CC) -o build/test/client test/client.c test/client.h src/logger.c src/logger.h

clean:
	rm build/server build/test/client