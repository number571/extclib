CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

SOURCES=crypto.c net.c type.c
HEADERS=crypto.h net.h type.h
OBJECTS=crypto.o net.o type.o

.PHONY: default compile link
default: compile link
compile: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o
