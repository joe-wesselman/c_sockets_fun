#ifndef REQUEST_H
#define REQUEST_H

#define METHOD_LEN 8
#define PATH_LEN 256
#define VERSION_LEN 16
#define HOST_LEN 128

#include <stddef.h>

typedef struct {
    char method[METHOD_LEN];
    char path[PATH_LEN];
    char version[VERSION_LEN];
    char host[HOST_LEN];
} HttpRequest;

void init_request(HttpRequest *req);
int parse_http_request(HttpRequest *req, const char *buffer);
void print_request(const HttpRequest *req);

#endif