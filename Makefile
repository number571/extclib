## use [gcc main.c extclib/extclib.o -lws2_32] when compile for windows (winsock2.h);

# CC=i686-w64-mingw32-gcc
# LN=ld -m i386pe

CC=gcc
LN=ld

CFLAGS=-Wall -std=c99 -save-temps

HEADERS=types/array.h types/bigint/bigd.h types/bigint/bigdigits.h types/bigint.h types/list.h \
	types/hashtab.h types/type.h types/dynamic.h types/tree.h types/string.h io.h net.h crypto.h \
	crypto/aes.h crypto/sha256.h 
SOURCES=types/array.c types/bigint/bigd.c types/bigint/bigdigits.c types/bigint.c types/list.c \
	types/hashtab.c types/type.c types/dynamic.c types/tree.c types/string.c io.c net.c crypto.c \
	crypto/aes.c crypto/sha256.c 
OBJECTS=array.o bigint.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o crypto.o type.o

.PHONY: default compile link clean
default: compile link
compile: $(SOURCES) $(HEADERS)
	# bigint.o
	$(CC) $(CFLAGS) -c types/bigint/bigd.c -o types/bigint/bigd.o
	$(CC) $(CFLAGS) -c types/bigint/bigdigits.c -o types/bigint/bigdigits.o
	$(CC) $(CFLAGS) -c types/bigint.c -o types/bigint/bigint.o
	$(LN) -r types/bigint/bigd.o types/bigint/bigdigits.o types/bigint/bigint.o -o bigint.o
	# crypto.o
	$(CC) $(CFLAGS) -c crypto/aes.c -o crypto/aes.o
	$(CC) $(CFLAGS) -c crypto/sha256.c -o crypto/sha256.o
	$(CC) $(CFLAGS) -c crypto.c -o crypto/crypto.o
	$(LN) -r crypto/aes.o crypto/sha256.o crypto/crypto.o -o crypto.o
	# array.o list.o hashtab.o io.o net.o dynamic.o tree.o string.o type.o
	$(CC) $(CFLAGS) -c types/array.c types/list.c types/hashtab.c types/dynamic.c types/tree.c \
		types/string.c types/type.c io.c net.c
link: $(OBJECTS)
	# extclib.o
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
	rm -f bigint/*.o bigint/*.i bigint/*.s
	rm -f crypto/*.o crypto/*.i crypto/*.s
	rm -f types/*.o types/*.i types/*.s
