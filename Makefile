CC=gcc
LN=ld

# CC=i686-w64-mingw32-gcc
# LN=ld -m i386pe

CFLAGS=-Wall -std=c99 # -save-temps

HEADERS=types/array.h types/bigint.h types/list.h types/hashtab.h types/type.h \
	types/dynamic.h types/tree.h io.h net.h crypto.h 
SOURCES=types/array.c types/bigint.c types/list.c types/hashtab.c types/type.c \
	types/dynamic.c types/tree.c io.c net.c crypto.c 
OBJECTS=array.o bigint.o list.o hashtab.o io.o net.o dynamic.o tree.o crypto.o type.o

.PHONY: default compile link clean
default: compile link
compile: $(SOURCES) $(HEADERS)
	# crypto.o, bigint.o, array.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o type.o
	$(CC) $(CFLAGS) -c crypto.c types/bigint.c types/array.c types/list.c types/hashtab.c types/dynamic.c \
		types/tree.c types/type.c io.c net.c
link: $(OBJECTS)
	# extclib.o
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
	rm -f types/*.o types/*.i types/*.s
