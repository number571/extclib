CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

SOURCES=crypto.c crypto/akey.c crypto/proof.c encoding.c database.c database/sqlite3.c \
	net.c net/http.c math/bigint.c math/rational.c type/hashtab.c type/list.c type/stack.c \
	math/bigint/mini-gmp.c math/bigint/mini-mpq.c
HEADERS=crypto.h crypto/akey.h crypto/proof.h encoding.h database.h database/sqlite3.h \
	net.h net/http.h math/bigint.h math/rational.h type/hashtab.h type/list.h type/stack.h \
	math/bigint/mini-gmp.h math/bigint/mini-mpq.h
OBJECTS=crypto.o crypto/akey.o crypto/proof.o encoding.o database.o database/sqlite3.o \
	net.o net/http.o math/bigint.o math/rational.o type/hashtab.o type/list.o type/stack.o \
	math/bigint/mini-gmp.o math/bigint/mini-mpq.o

.PHONY: default compile link
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o 
clean:
	rm -f *.o net/*.o type/*.o
