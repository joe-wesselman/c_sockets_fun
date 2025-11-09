#ifndef SERVER_H
#define SERVER_H

#include "request.h"

int start_server(int port, int (*handler)(int client_fd, const HttpRequest *req));

#endif