#include "server.h"
#include "response.h"

#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define BACKLOG 16
#define BUFF_SIZE 4096

static void die(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int start_server(int port, int (*handler)(int client_fd, const HttpRequest *req)){
    signal(SIGPIPE, SIG_IGN);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        die("socket dead");
    }
    int yes = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
        die("set sock opt died");
    }
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        die("bind failure");
    }
    if(listen(server_fd, BACKLOG) < 0){
        die("listening failure");
    }

    printf("Server listening on 0.0.0.0:%d\n", port);

    while(1){
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        if (client_fd < 0){
            perror("unable to accept negative client file descriptor");
            continue;
        }
        printf("[INFO] Connection from client: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        char buff[BUFF_SIZE];
        ssize_t bytes_read = read(client_fd, buff, sizeof(buff) -1);
        if (bytes_read <= 0){
            close(client_fd);
            continue;
        }
        buff[bytes_read] = '\0'; 
        HttpRequest req;
        init_request(&req);
        if(parse_http_request(&req, buff) < 0){
            fprintf(stderr, "[WARNING] Bad request from %s\n", inet_ntoa(client_address.sin_addr));
            send_bad_request(client_fd);
            close(client_fd);
        }
        handler(client_fd, &req);
        close(client_fd);
    }
    close(server_fd);
    return 0;
}
