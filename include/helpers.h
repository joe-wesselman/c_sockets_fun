#ifndef HELPERS_H
#define HELPERS_H
#include <stdlib.h> 
char *read_file(const char *fname, size_t *out_size);
const char *get_mime_type(const char *fname);
#endif