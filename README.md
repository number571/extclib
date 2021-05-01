# extclib
> Extented C library. Version 0.5.0.

### Implemented:
1. Net: tcp, http;
2. Type: bigint, hashtab, list, stack;
3. Crypto: rand, hash, encrypt;

### Compile library:
```
$ make -C extclib/
$ cc main.c extclib/extclib.o -o main
```

### Example (ECHO server):
```c
#include "extclib/net.h"

#include <stdio.h>

#define ADDRESS "0.0.0.0", 8080

int main(void) {
	int n;
	char buffer[BUFSIZ];
	net_t *listener, *conn;
	
	listener = net_listen(ADDRESS);
	while(1) {
		conn = net_accept(listener);
		n = net_recv(conn, buffer, BUFSIZ);
		net_send(conn, buffer, n);
		net_close(conn);
	}

	net_close(listener);
	return 0;
}
```

### Example (ECHO client):
```c
#include "extclib/net.h"

#include <stdio.h>
#include <string.h>

#define ADDRESS "127.0.0.1", 8080

int main(void) {
	net_t *conn;
	char buffer[BUFSIZ];
	const char *message = "hello, world!";

	conn = net_connect(ADDRESS);
	net_send(conn, message, strlen(message)+1);
	net_recv(conn, buffer, BUFSIZ);

	printf("%s\n", buffer);
	net_close(conn);
	return 0;
}
```
