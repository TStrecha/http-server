#include <stdio.h>
#include <winsock2.h>
#include <winsock.h>
#include <string.h>

#include "server.h"
#include "request.h"
#include "response.h"
#include "lib/error.h"
#include "lib/map.h"
#include "lib/lib.h"
#include "log/log.h"

void handleRequest(Request* request, Response* response);

int start_server(Server* server) {
    WSADATA wsaData;
    int wsaStartupStatus = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(wsaStartupStatus != NO_ERROR) {
        log_wsa_err("WSA initialization failed.");
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == INVALID_SOCKET) {
        log_wsa_err("Socket creation failed.");

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    struct sockaddr_in saServer = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(server->ip),
        .sin_port = htons(server->port)
    };

    int bindRes = bind(serverSocket, (SOCKADDR *) &saServer, sizeof(saServer));
    if(bindRes != NO_ERROR) {
        log_wsa_err("Binding failed.");

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    int listenRes = listen(serverSocket, 5);
    if(listenRes != NO_ERROR) {
        log_wsa_err("Listening failed.");

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    log_info("Waiting for incoming connections at %s:%d", server->ip, server->port);

    while(1) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == INVALID_SOCKET) {
            log_wsa_err("You messed up, but when accepting."); 
        } else {
            log_info("(CL%d) Client connected.", clientSocket);

            Request* request = readRequest(clientSocket);
            StrHashMap* res_headers = init_shmap();
            Response response = {
                .headers = res_headers
            };

            if(request != NULL) {
                handleRequest(request, &response);
            } else {
                response.statusCode = INTERNAL_SERVER_ERROR;
            }

            int responseLength = sendResponse(clientSocket, &response);

            log_info("(CL%d) Responded with %d bytes and status code %s", clientSocket, 
                responseLength, getStatusCodeAsStr(response.statusCode));
            log_info("(CL%d) Shutting down the client socket.", clientSocket);

            shutdown(clientSocket, SD_SEND);
            closesocket(clientSocket);
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return NOERR;
}

void handleRequest(Request* request, Response* response) {
    response->statusCode = OK;
    response->contentType = CT_APP_JSON;
    response->content = fstring("{\"path\": \"%s\", \"content\": \"%s\"}", request->req_line->path, request->body);
}