LN=ld
CC=gcc
.PHONY: default compile
default: compile
compile: bigint/bigd.c bigint/bigdigits.c bigint.c list.c hashtab.c io.c net.c stack.c tree.c type.c
	$(CC) bigint/bigd.c -c -o bigint/bigd.o
	$(CC) bigint/bigdigits.c -c -o bigint/bigdigits.o
	$(CC) bigint.c -c -o bigint/bigint.o
	$(LN) -r bigint/bigd.o bigint/bigdigits.o bigint/bigint.o -o bigint.o
	$(CC) -c list.c hashtab.c io.c net.c stack.c tree.c type.c
	$(LN) -r bigint.o list.o hashtab.o io.o net.o stack.o tree.o type.o -o extclib.o
