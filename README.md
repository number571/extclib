# extclib
> Extended C library. Version: 0.1.0.

### Implemented:
1. Types: Array, BigInt, Dynamic, HashTab, List, Tree;
2. Crypto: AES[ECB, CBC], RSA[OAEP, SIGN], SHA256, HMAC[SHA256], RAND;
3. Encoding: Base64, JSON;
4. Database: SQLite;
5. Net: TCP(Linux[arpa/inet.h], Windows[winsock2.h]), HTTP;

### Used libraries:
1. gmp: [gmplib.org](https://gmplib.org/);
2. openssl: [openssl.org](https://www.openssl.org/);
3. cJSON: [cJSON.git](https://github.com/DaveGamble/cJSON.git/);
4. sqlite3: [sqlite.org](https://www.sqlite.org/);

### Compile library:
```
/* install 'gmp', 'openssl', 'cJSON', 'sqlite3' libraries before compilation */
$ make -C extclib/

/* linux compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto -lcjson -lsqlite3

/* windows compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto -lcjson -lsqlite3 -lws2_32
```

### Usage restrictions:
1. Recursive hash tables, trees, lists, arrays are not allowed. Cross-recursive is dangerous;
2. Values (and/or keys) in hash tables, trees, lists, arrays are freed up automatically when deleting or replacing an object's value;

### Example (HTTP server):
```c
#include "extclib/http.h"

void _index_page(int conn, Request *req);
void _about_page(int conn, Request *req);

int main(void) {
    HTTP *serve = new_http("127.0.0.1:7545");

    handle_http(serve, "/", _index_page);
    handle_http(serve, "/about", _about_page);

    listen_http(serve);
    return 0;
}

void _index_page(int conn, Request *req) {
    parsehtml_http(conn, "index.html");
}

void _about_page(int conn, Request *req) {
    parsehtml_http(conn, "about.html");
}
```
