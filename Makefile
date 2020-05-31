LN=ld
CC=gcc

HEADERS=array.h bigint/bigd.h bigint/bigdigits.h bigint.h list.h hashtab.h io.h net.h dynamic.h tree.h string.h type.h
SOURCES=array.c bigint/bigd.c bigint/bigdigits.c bigint.c list.c hashtab.c io.c net.c dynamic.c tree.c string.c type.c
OBJECTS=array.o bigint.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o type.o

.PHONY: default compile link clean
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) -c bigint/bigd.c -o bigint/bigd.o
	$(CC) -c bigint/bigdigits.c -o bigint/bigdigits.o
	$(CC) -c bigint.c -o bigint/bigint.o
	$(LN) -r bigint/bigd.o bigint/bigdigits.o bigint/bigint.o -o bigint.o
	$(CC) -c array.c list.c hashtab.c io.c net.c dynamic.c tree.c string.c type.c
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o 
	rm -f bigint/*.o
