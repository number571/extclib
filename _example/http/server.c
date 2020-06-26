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
