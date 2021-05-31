CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

SOURCES=crypto.c crypto/akey.c crypto/proof.c encoding.c database.c database/sqlite3.c \
	net.c net/http.c type/bigint.c type/hashtab.c type/list.c type/stack.c \
	type/bigint/mini-gmp.c type/bigint/mini-mpq.c
HEADERS=crypto.h crypto/akey.h crypto/proof.h encoding.h database.h database/sqlite3.h \
	net.h net/http.h type/bigint.h type/hashtab.h type/list.h type/stack.h \
	type/bigint/mini-gmp.h type/bigint/mini-mpq.h
OBJECTS=crypto.o crypto/akey.o crypto/proof.o encoding.o database.o database/sqlite3.o \
	net.o net/http.o type/bigint.o type/hashtab.o type/list.o type/stack.o \
	type/bigint/mini-gmp.o type/bigint/mini-mpq.o

.PHONY: default compile link
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o 
clean:
	rm -f *.o net/*.o type/*.o
