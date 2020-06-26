CC=gcc
LN=ld

# CC=i686-w64-mingw32-gcc
# LN=ld -m i386pe

CFLAGS=-Wall -std=c99 # -save-temps

HEADERS=types/array.h types/bigint.h types/list.h types/hashtab.h types/type.h \
	types/dynamic.h types/tree.h types.h io.h net.h http.h crypto/aes.h crypto/rsa.h \
	crypto/rand.h crypto/sha256.h crypto/hmac256.h crypto.h encoding/base64.h \
	encoding/json.h encoding.h database/sqlite.h database.h
SOURCES=types/array.c types/bigint.c types/list.c types/hashtab.c types/type.c \
	types/dynamic.c types/tree.c io.c net.c http.c crypto/aes.c crypto/rsa.c \
	crypto/rand.c crypto/sha256.c crypto/hmac256.c encoding/base64.c \
	encoding/json.c database/sqlite.c
OBJECTS=array.o bigint.o list.o hashtab.o io.o net.o http.o dynamic.o tree.o aes.o rsa.o \
	rand.o sha256.o hmac256.o type.o base64.o json.o sqlite.o

.PHONY: default compile link clean
default: compile link
compile: $(SOURCES) $(HEADERS)
	# array.o, bigint.o, list.o hashtab.o io.o net.o dynamic.o tree.o aes.o rsa.o
	# rand.o sha256.o hmac256.o type.o base64.o json.o sqlite.o
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	# extclib.o
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
