CC=gcc
.PHONY: default compile
default: compile
compile: hashtab.c io.c net.c stack.c tree.c type.c
	$(CC) -c hashtab.c io.c net.c stack.c tree.c type.c
