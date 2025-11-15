#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "helpers.h"

void send_response(int client_fd, const char *status, const char *mimetype, const char* body){
    size_t body_len = body ? strlen(body) : 0;
    char header[512];
    snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status, mimetype, body_len);
    send(client_fd, header, strlen(header), 0);
    if (body_len > 0){
        send(client_fd, body, body_len, 0);
    }
}

void send_bad_request(int client_fd){
    char *body = "<h1>400 Bad Request</h1>";
    size_t body_len = strlen(body);
    char header[512];
    snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        "400", "text/html", body_len);
    send(client_fd, header, strlen(header), 0);
    if (body_len > 0){
        send(client_fd, body, body_len, 0);
    }
}

void send_not_found(int client_fd){
    char *body = "<h1>404 Not Found</h1>";
    size_t body_len = strlen(body);
    char header[512];
    snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        "404", "text/html", body_len);
    send(client_fd, header, strlen(header), 0);
    if (body_len > 0){
        send(client_fd, body, body_len, 0);
    }
}

void send_internal_error(int client_fd){
    char *body = "<h1>500 Internal Server Error</h1>";
    size_t body_len = strlen(body);
    char header[512];
    snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        "500", "text/html", body_len);
    send(client_fd, header, strlen(header), 0);
    if (body_len > 0){
        send(client_fd, body, body_len, 0);
    }
}

void send_static_file(int client_fd, const char *path){
    size_t filesize;
    const char *prefix =".";
    int needed_space = snprintf(NULL, 0, "%s%s", prefix, path);
    char *normalizedpath = malloc(needed_space + 1);
    if(!normalizedpath){ perror("malloc error"); exit(1);}
    if(!(strncmp(path, ".", 1) == 0)){
        snprintf(normalizedpath, needed_space + 1, "%s%s", prefix, path);
    }
    else{
        snprintf(normalizedpath, needed_space, "%s", path);
    }
    const char *mimetype = get_mime_type(normalizedpath);
    char *filecontent = read_file(normalizedpath, &filesize);
    if(!filecontent){
        send_not_found(client_fd);
    }
    else{
        send_response(client_fd, "200", mimetype, filecontent);
    }
    free(normalizedpath);
}