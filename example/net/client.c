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
