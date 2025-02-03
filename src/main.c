#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm/request.h"
#include "comm/response.h"
#include "log/log.h"
#include "comm/server.h"
#include "lib/lib.h"

void handle_root(Request* request, Response* response);
void handle_html(Request* request, Response* response);

int main(int argc, char** argv) {
    short port = 80;
    char* ip = "127.0.0.1";
    int log_level = LOG_INFO;

    for(int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if(strcmp(arg, "-p") == 0 || strcmp(arg, "--port") == 0) {
            if(i == argc - 1) {
                printf("Wrong command usage.\n");
                return 1;
            }

            char* param = argv[i + 1];
            if(!isNumber(param)) {
                printf("Wrong command usage.\n");
                return 1;
            }
            port = atoi(param);
            i += 1;
        }

        if(strcmp(arg, "-h") == 0 || strcmp(arg, "--host") == 0) {
            if(i == argc - 1) {
                printf("Wrong command usage.\n");
                return 1;
            }

            ip = argv[i + 1];
            i += 1;
        }

        if(strcmp(arg, "-ll") == 0 || strcmp(arg, "--log-level") == 0) {
            if(i == argc - 1) {
                printf("Wrong command usage.\n");
                return 1;
            }
            
            char* param = argv[i + 1];

            if(strcmp(param, "TRACE") == 0) {
                log_level = LOG_TRACE;
            } else if(strcmp(param, "DEBUG") == 0) {
                log_level = LOG_DEBUG;
            } else if(strcmp(param, "INFO") == 0) {
                log_level = LOG_INFO;
            } else if(strcmp(param, "WARN") == 0) {
                log_level = LOG_WARN;
            } else  if(strcmp(param, "ERROR") == 0) {
                log_level = LOG_ERROR;
            } else  if(strcmp(param, "FATAL") == 0) {
                log_level = LOG_FATAL;
            }
        }
    }

    log_set_level(log_level);

    Server server = create_server();
    server.ip = ip;
    server.port = port;
    add_route(&server, GET, "/", &handle_root);
    add_route(&server, GET, "/html", &handle_html);

    return start_server(&server);
}

void handle_root(Request* request, Response* response) {
    response->statusCode = OK;
    response->contentType = CT_APP_JSON;
    response->content = fstring("{\"path\": \"%s\", \"client_content\": \"%s\"}", request->req_line->path, request->body);
}

void handle_html(Request* request, Response* response) {
    response->statusCode = OK;
    response->contentType = CT_TEXT_HTML;
    response->content = fstring("<html><body><h2>TEST: %s</h2></body></html>", request->body);
}