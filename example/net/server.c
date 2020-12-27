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
	}

	net_close(listener);
	return 0;
}
