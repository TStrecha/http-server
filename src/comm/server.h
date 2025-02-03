#ifndef _SERVER_H_
#define _SERVER_H_

#include "request.h"
#include "response.h"

typedef void (*RouteHandler) (Request*, Response*);

typedef struct {
    StrHashMap* routes;
    RouteHandler notFoundHandler;
    RouteHandler serverErrorHandler;
    short port;
    char* ip;
} Server;

Server create_server();
void add_route(Server* self, RequestMethod method, char* path, RouteHandler handler);
int start_server(Server* self);

#endif