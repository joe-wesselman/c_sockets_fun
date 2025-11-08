#include "request.h"
#include <stdio.h>
#include <string.h>


void init_request(HttpRequest *req){
    memset(req, 0, sizeof(*req));
}

void print_request(const HttpRequest *req){
    printf("Method: %s\nPath: %s\nVersion: %s\n", req->method, req->path, req->version);
}

int parse_http_request(HttpRequest *req, const char *buffer){
    if(buffer == NULL){
        return -1;
    }
    const char *linend = strstr(buffer,"\r\n");
    if(!linend){
        return -1;
    }
    char line[512];
    size_t content_len = linend - buffer;
    if(content_len >= sizeof(line)){
        return -1; // if send me too much I cannot handle it and reject
    }

    strncpy(line, buffer, content_len); // copy the first line from buffer into our line
    // copy into our values
    if(sscanf(line, "%7s %255s %15s", req->method, req->path, req->version) != 3){
        return -1;
    }
    
    return 0;
}