# extclib
> Extented c library. Version 0.2.0.

### Implemented:
1. Net: TCP connections;
2. Type: List, Hashtab;
3. Crypto: AES-256CBC, RSA-OAEP, RSA-SIGN, SHA-256, RAND;

### Used libraries:
1. openssl: [openssl.org](https://www.openssl.org/);

### Compile library:
```
/* install 'openssl' library before compilation */
$ make -C extclib/

/* unix compile: */
$ cc main.c extclib/extclib.o -o main -lcrypto

/* windows compile: */
$ cc main.c extclib/extclib.o -o main -lcrypto -lws2_32
```

### Example (ECHO server):
```c
#include "extclib/net.h"

#include <stdio.h>

#define ADDRESS "0.0.0.0", 8080

int main(void) {
	int n;
	char buffer[BUFSIZ];
	net_conn *listener, *conn;
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
	net_conn *conn;
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
