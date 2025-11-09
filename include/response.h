#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h>

void send_response(int client_fd, const char *status, const char *mimetype, const char* body);

void send_file(int client_df, const char *filepath);

void send_not_found(int client_fd);
void send_bad_request(int client_fd);
void send_internal_error(int client_fd);

#endif