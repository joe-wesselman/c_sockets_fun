#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 4096
#define PORT 5050
#define BACKLOG 16

static void die(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    
    int port = (argc > 1) ? atoi(argv[1]) : PORT;

    signal(SIGPIPE, SIG_IGN);

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) die("socket");

    int yes = 1;
    if (setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
        die("setsockopt SO_REUSEADDR");
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    addr.sin_port = htons(port);

    if (bind(srv, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        die("bind failure");
    }

    if(listen(srv, BACKLOG) < 0){
        die("listen failure");
    }

    printf("Echo server listening on 0.0.0.0:%d\n", port);

    for(;;){
        struct sockaddr_in clientAddr;

        socklen_t clen = sizeof(clientAddr);

        int cli = accept(srv, (struct sockaddr*)&clientAddr, &clen);
        if (cli < 0){
            if (errno == EINTR) continue;
            die("accept");
        }
        char clientip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientip, sizeof(clientip));
        printf("Client connected from %s:%d\n", clientip, ntohs(clientAddr.sin_port));
        char buf[BUFF_SIZE];
        for(;;){
            ssize_t n = recv(cli, buf, sizeof(buf), 0);
            if (n == 0){
                printf("client closed conn\n");
                break;
            }
            if (n < 0){
                perror("recv");
                break;
            }
            printf("%d-------\n", (int) n);
            printf("%.*s",(int)n,buf);
            fflush(stdout);
        }
    }

    close(srv);

    return 0;


}