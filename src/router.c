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

RouteHandler get_handler(const char *path){
    for(int i = 0; i < route_count; i++){
        if(strcmp(routes[i].path, path) == 0){
            return routes[i].handler;
        }
    }
    return NULL;
}

int router_dispatch(int client_fd, const HttpRequest *req){
    RouteHandler handler = get_handler(req->path);
    if(handler){
        handler(client_fd, req);
    }
    else if (strncmp(req->path, "/assets/", 8) == 0){
        // meaning we have a request for some asset
        send_static_file(client_fd, req->path);
    }
    else{
        send_not_found(client_fd);
    }
    return 0;
}