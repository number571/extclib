LN=ld
CC=gcc
CFLAGS=-Wall -std=c99

HEADERS=array.h bigint/bigd.h bigint/bigdigits.h bigint.h list.h hashtab.h io.h net.h dynamic.h tree.h string.h crypto.h crypto/aes.h crypto/funcs.h type.h
SOURCES=array.c bigint/bigd.c bigint/bigdigits.c bigint.c list.c hashtab.c io.c net.c dynamic.c tree.c string.c crypto.c crypto/aes.c crypto/funcs.c type.c
OBJECTS=array.o bigint.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o crypto.o type.o

.PHONY: default compile link clean
default: compile link
compile: $(SOURCES) $(HEADERS)
	# bigint.o
	$(CC) $(CFLAGS) -c bigint/bigd.c -o bigint/bigd.o
	$(CC) $(CFLAGS) -c bigint/bigdigits.c -o bigint/bigdigits.o
	$(CC) $(CFLAGS) -c bigint.c -o bigint/bigint.o
	$(LN) -r bigint/bigd.o bigint/bigdigits.o bigint/bigint.o -o bigint.o
	# crypto.o
	$(CC) $(CFLAGS) -c crypto/aes.c -o crypto/aes.o
	$(CC) $(CFLAGS) -c crypto/funcs.c -o crypto/funcs.o
	$(CC) $(CFLAGS) -c crypto.c -o crypto/crypto.o
	$(LN) -r crypto/aes.o crypto/funcs.o crypto/crypto.o -o crypto.o
	# array.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o type.o
	$(CC) $(CFLAGS) -c array.c list.c hashtab.c io.c net.c dynamic.c tree.c string.c type.c
link: $(OBJECTS)
	# extclib.o
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o 
	rm -f bigint/*.o
	rm -f crypto/*.o
