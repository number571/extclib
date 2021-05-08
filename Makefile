CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

SOURCES=crypto.c crypto/akey.c net.c net/http.c type/bigint.c type/hashtab.c type/list.c type/stack.c \
	type/mini-gmp/mini-gmp.c type/mini-gmp/mini-mpq.c
HEADERS=crypto.h crypto/akey.h net.h net/http.h type/bigint.h type/hashtab.h type/list.h type/stack.h \
	type/mini-gmp/mini-gmp.h type/mini-gmp/mini-mpq.h
OBJECTS=crypto.o crypto/akey.o net.o net/http.o type/bigint.o type/hashtab.o type/list.o type/stack.o \
	type/mini-gmp/mini-gmp.o type/mini-gmp/mini-mpq.o

.PHONY: default compile link
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)

link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o net/*.o type/*.o
