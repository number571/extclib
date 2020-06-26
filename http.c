#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

#include "net.h"
#include "types/hashtab.h"

typedef struct Request {
    char method[16];
    char path[2048];
    uint8_t state;
    size_t index;
} Request;

typedef struct HTTP {
    char *host;
    int32_t len;
    int32_t cap;
    void(**funcs)(int conn, Request *req);
    HashTab *tab;
} HTTP;

static Request _new_request(void);
static void _parse_request(Request *request, char *buffer, size_t size);
static void _null_reqdata(Request *request);
static int8_t _switch_http(HTTP *http, int conn, Request *request);
static void _page404_http(int conn);

extern HTTP *new_http(char *address) {
    HTTP *http = (HTTP*)malloc(sizeof(HTTP));
    http->cap = 1000;
    http->len = 0;
    http->host = (char*)malloc(sizeof(char)*strlen(address)+1);
    http->funcs = (void(**)(int conn, Request *req))malloc(http->cap * (sizeof (void(*)(int conn, Request *req))));
    http->tab = new_hashtab(http->cap, STRING_TYPE, DECIMAL_TYPE);
    strcpy(http->host, address);
    return http;
}

extern void free_http(HTTP *http) {
    free_hashtab(http->tab);
    free(http->host);
    free(http);
}

extern int8_t handle_http(HTTP *http, char *path, void(*handle)(int conn, Request *req)) {
    set_hashtab(http->tab, string(path), decimal(http->len));
    http->funcs[http->len] = handle;
    http->len += 1;
    if (http->len == http->cap) {
        http->cap <<= 1;
        http->funcs = (void(**)(int conn, Request *req))realloc(http->funcs, http->cap * (sizeof (void(*)(int conn, Request *req))));
    }
    return 0;
}

extern int8_t listen_http(HTTP *http) {
    int listener = listen_net(http->host);
    if (listener < 0) {
        return 1;
    }
    while(1) {
        int conn = accept_net(listener);
        if (conn < 0) {
            continue;
        }
        Request req = _new_request();
        while(1) {
            char buffer[BUFSIZ] = {0};
            int n = recv_net(conn, buffer, BUFSIZ);
            if (n < 0) {
                break;
            }
            _parse_request(&req, buffer, n);
            if (n != BUFSIZ) {
                break;
            }
        }
        _switch_http(http, conn, &req);
        close_net(conn);
    }
    close_net(listener);
    return 0;
}

extern void parsehtml_http(int conn, char *filename) {
    char buffer[BUFSIZ] = "HTTP/1.1 200 OK\r\n\n";
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}

static Request _new_request(void) {
    return (Request){
        .method = {0},
        .path = {0},
        .index = 0,
        .state = 0,
    };
}

static void _parse_request(Request *request, char *buffer, size_t size) {
    const size_t maxsize_method = 16;
    const size_t maxsize_path = 2048;
    for (size_t i = 0; i < size; ++i) {
        switch(request->state) {
            case 0:
                if (buffer[i] == ' ' || request->index == maxsize_method) {
                    request->method[request->index] = '\0';
                    _null_reqdata(request);
                    continue;
                } 
                request->method[request->index] = buffer[i];
            break;
            case 1: 
                if (buffer[i] == ' ' || request->index == maxsize_path) {
                    request->path[request->index] = '\0';
                    _null_reqdata(request);
                    continue;
                } 
                request->path[request->index] = buffer[i];
            break;
            default: return;
        }
        request->index += 1;
    }
}

static void _null_reqdata(Request *request) {
    request->state += 1;
    request->index = 0;
}

static int8_t _switch_http(HTTP *http, int conn, Request *request) {
    if (!in_hashtab(http->tab, string(request->path))) {
        size_t length = strlen(request->path);
        if (length == 0) {
            _page404_http(conn);
            return 1;
        }
        int32_t index = length-1;
        request->path[index] = '\0';
        for (; request->path[index] != '/' && index > 0; --index) {
            request->path[index] = '\0';
        }
        return _switch_http(http, conn, request);
    }
    int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
    http->funcs[index](conn, request);
    return 0;
}

static void _page404_http(int conn) {
    char *header = "HTTP/1.1 404 Not Found\r\n\nnot found";
    size_t headsize = strlen(header);
    send_net(conn, header, headsize);
}
