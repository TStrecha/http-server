#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <winsock.h>
#include "lib/map.h"

#define READ_BUFFER_SIZE (10 * 1024 * 1024)

typedef enum {
    GET, POST
} RequestMethod;

typedef enum {
    HTTP_1_1
} Protocol;

typedef struct {
    RequestMethod method;
    char* path;
    Protocol protocol;
} RequestLine;

typedef struct {
    char* method;
    char* path;
    char* protocol;
} RequestLineRaw;

typedef struct {
    RequestLine* req_line;
    StrHashMap* headers;
    char* body;
} Request;

Request* readRequest(SOCKET clientSocket);
int parseRequestLineRaw(RequestLineRaw* out, char* requestLine);
char* getMethodAsStr(RequestMethod method);

#endif