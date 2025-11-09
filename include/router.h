#ifndef ROUTER_H
#define ROUTER_H

#include "request.h"

typedef void (*RouteHandler) (int client_fd, const HttpRequest *req);

typedef struct {
    const char *path;
    RouteHandler handler;
} Route;

void router_init(void);

void router_register(const char *path, RouteHandler handler);

int router_dispatch(int client_fd, const HttpRequest *req);

#endif