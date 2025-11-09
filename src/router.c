#include <stdio.h>
#include <string.h>
#include "router.h"
#include "response.h"

#define MAX_ROUTES 64

static Route routes[MAX_ROUTES];
static int route_count = 0;

void router_init(void){
    route_count = 0;
}

void router_register(const char *path, RouteHandler handler){
    if (route_count >= MAX_ROUTES){
        fprintf(stderr, "Router: max route count reached %d\n", MAX_ROUTES);
        return;
    }
    routes[route_count].path = path;
    routes[route_count].handler = handler;
    route_count++;
}

int router_dispatch(int client_fd, const HttpRequest *req){
    for(int i = 0; i < route_count; i++){
        if(strcmp(routes[i].path,req->path) == 0){
            routes[i].handler(client_fd, req);
            return 0;
        }
    }
    send_not_found(client_fd);
    return 0;
}