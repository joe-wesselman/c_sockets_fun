#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char *get_mime_type(const char *fname){
    const char *extension = strrchr(fname, '.');
    if(!extension) return "text/plain";
    if (strcmp(extension, ".html") == 0) return "text/html";
    if (strcmp(extension, ".css")  == 0) return "text/css";
    if (strcmp(extension, ".js")   == 0) return "application/javascript";
    if (strcmp(extension, ".png")  == 0) return "image/png";
    if (strcmp(extension, ".jpg")  == 0 || strcmp(extension, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(extension, ".json") == 0) return "application/json";
    return "application/octet-stream";
}