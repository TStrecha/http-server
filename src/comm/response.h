#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <winsock.h>

#include "lib/map.h"
#include "request.h"
#include "response.h"

#define CT_TEXT_PLAIN "text/plain"
#define CT_TEXT_HTML "text/html"
#define CT_APP_JSON "application/json"

typedef enum {
    OK = 200,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
} StatusCode;

typedef struct {
    StatusCode statusCode;
    StrHashMap* headers;
    char* contentType;
    char* content;
} Response;

typedef void (*RouteHandler) (Request*, Response*);

int sendResponse(SOCKET clientSocket, Response* response);
char* getStatusCodeAsStr(StatusCode code);

#endif