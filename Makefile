CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

SOURCES=crypto.c net.c net/http.c type/hashtab.c type/list.c type/stack.c
HEADERS=crypto.h net.h net/http.h type/hashtab.h type/list.h type/stack.h
OBJECTS=crypto.o net.o net/http.o type/hashtab.o type/list.o type/stack.o

.PHONY: default compile link
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)

link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o net/*.o type/*.o
