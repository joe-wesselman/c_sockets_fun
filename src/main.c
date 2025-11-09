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

#define BUFF_SIZE 4096
#define PORT 5050
#define BACKLOG 16

char *read_file(const char *fname, size_t *out_size)
{
    FILE *fp = fopen(fname, "rb");
    if (!fp)
    {
        perror("fopen");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *buffer = malloc((size_t)len + 1);
    if (!buffer)
    {
        perror("malloc failure");
        fclose(fp);
        return NULL;
    }
    size_t n = fread(buffer, 1, (size_t)len, fp);
    buffer[n] = '\0'; // terminate str
    fclose(fp);
    return buffer;
}

void handle_root(int fd, const HttpRequest *req){
    size_t simplehtmlsize;
    char *simplehtml = read_file("assets/simple.html", &simplehtmlsize);
    send_response(fd, "200", "text/html", simplehtml);
}

void handle_ping(int fd, const HttpRequest *req){
    size_t pongsize;
    char *pongres = read_file("assets/pong.html", &pongsize);
    send_response(fd, "200", "text/html", pongres);
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