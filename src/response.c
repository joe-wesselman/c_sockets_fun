#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>

// static const char *get_mime_type(const char *fname){
//     const char *extenstion = strrchr(fname, '.');
//     if(!extenstion) return "text/plain";
//     if (strcmp(extenstion, ".html") == 0) return "text/html";
//     if (strcmp(extenstion, ".css")  == 0) return "text/css";
//     if (strcmp(extenstion, ".js")   == 0) return "application/javascript";
//     if (strcmp(extenstion, ".png")  == 0) return "image/png";
//     if (strcmp(extenstion, ".jpg")  == 0 || strcmp(extenstion, ".jpeg") == 0) return "image/jpeg";
//     if (strcmp(extenstion, ".json") == 0) return "application/json";
//     return "application/octet-stream";
// }

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