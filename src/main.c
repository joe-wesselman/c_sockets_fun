#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "request.h"

#define BUFF_SIZE 4096
#define PORT 5050
#define BACKLOG 16

static void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

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

char *build_header(char *htmlcontent)
{
    char *header = malloc(256);
    int content_length = strlen(htmlcontent);
    snprintf(header, 256,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             content_length);
    return header;
}

int main(int argc, char **argv)
{

    int port = (argc > 1) ? atoi(argv[1]) : PORT;

    signal(SIGPIPE, SIG_IGN);

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0)
        die("socket");

    int yes = 1;
    if (setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        die("setsockopt SO_REUSEADDR");
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    addr.sin_port = htons(port);

    if (bind(srv, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        die("bind failure");
    }

    if (listen(srv, BACKLOG) < 0)
    {
        die("listen failure");
    }

    printf("Echo server listening on 0.0.0.0:%d\n", port);
    size_t simplehtmlsize;
    char *simplehtml = read_file("assets/simple.html", &simplehtmlsize);
    size_t fofsize;
    char *fofcontent = read_file("assets/404.html", &fofsize);
    size_t pongsize;
    char *pongcontent = read_file("assets/pong.html", &pongsize);
    char *header = build_header(simplehtml);
    while (1)
    {
        struct sockaddr_in clientAddr;
        socklen_t clen = sizeof(clientAddr);
        int client_fd = accept(srv, (struct sockaddr *)&clientAddr, &clen);
        printf("accepted\n");
        if (client_fd < 0)
        {
            perror("Accept traffic failed\n");
            close(srv);
            free(simplehtml);
            free(header);
            return 1;
        }
        
        char buff[2048];
        read(client_fd, buff, sizeof(buff) -1);
        HttpRequest req;
        init_request(&req);
        
        if (parse_http_request(&req, buff) == 0){
            print_request(&req);
        }
        else{
            printf("Unable to parse request");
            continue;
        }
        send(client_fd, header, strlen(header), 0);
        if(strcmp(req.path, "/") == 0){
            send(client_fd, simplehtml, strlen(simplehtml), 0);
        }
        else if(strcmp(req.path, "/ping") == 0){
            send(client_fd, pongcontent, strlen(pongcontent), 0);
        }
        else{
            printf("GOT PATH: %s", req.path);
            send(client_fd, fofcontent, strlen(fofcontent), 0);
        }
        
        printf("\nsent content\n");

    }
    free(simplehtml);
    free(header);
    close(srv);

    return 0;
}