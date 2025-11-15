#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "request.h"
#include "router.h"
#include "server.h"
#include "response.h"
#include "helpers.h"

#define BUFF_SIZE 4096
#define PORT 5050
#define BACKLOG 16



void handle_root(int fd, const HttpRequest *req){
    size_t homehtmlsize;
    const char *filename = "assets/html/home.html";
    char *homehtml = read_file(filename, &homehtmlsize);
    send_response(fd, "200", get_mime_type(filename), homehtml);
}

void handle_ping(int fd, const HttpRequest *req){
    size_t pongsize;
    const char *filename = "assets/html/pong.html";
    char *pongres = read_file(filename, &pongsize);
    send_response(fd, "200", get_mime_type(filename), pongres);
}

int main(int argc, char **argv)
{
    int port = (argc > 1) ? atoi(argv[1]) : PORT;
    router_init();
    router_register("/", handle_root);
    router_register("/ping", handle_ping);
    start_server(port, router_dispatch);
    return 0;
}