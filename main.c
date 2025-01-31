#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm/server.h"
#include "lib/lib.h"

int main(int argc, char** argv) {
    short port = 80;
    char* ip = "127.0.0.1";

    for(int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if(strcmp(arg, "-p") == 0 || strcmp(arg, "--port") == 0) {
            if(i == argc - 1) {
                printf("Wrong command usage.\n");
                return 0;
            }

            char* param = argv[i + 1];
            if(!isNumber(param)) {
                printf("Wrong command usage.\n");
                return 0;
            }
            port = atoi(param);
            i += 1;
        }

        if(strcmp(arg, "-h") == 0 || strcmp(arg, "--host") == 0) {
            if(i == argc - 1) {
                printf("Wrong command usage.\n");
                return 0;
            }

            ip = argv[i + 1];
            i += 1;
        }
    }

    Server server = {
        .port = port,
        .ip = ip,
    };
    start_server(&server);
}