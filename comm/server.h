#ifndef _SERVER_H_
#define _SERVER_H_

typedef struct {
    short port;
    char* ip;
} Server;

int start_server(Server* server);

#endif