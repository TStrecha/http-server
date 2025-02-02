#include "response.h"
#include "lib/lib.h"
#include "lib/mem.h"
#include "log/log.h"

int sendResponse(SOCKET clientSocket, Response* response) {
    char* payload;
    char* statusCode = getStatusCodeAsStr(response->statusCode);
    char* headerString = "";


    if(response->content != NULL) {
        insert_sshmap(response->headers, "Content-Length", fstring("%d", strlen(response->content)));
        insert_sshmap(response->headers, "Content-Type", response->contentType);
    }
    StrStrNode** headers = get_all_sshmap(response->headers);

    for (int i = 0; i < response->headers->elements; i++) {
        StrStrNode* header = headers[i];
        headerString = fstring("%s%s: %s\r\n", headerString, header->key, header->value);
    }
    
    if(response->content != NULL) {
        payload = fstring(  "HTTP/1.1 %s\r\n" 
                            "%s"
                            "\r\n"
                            "%s", statusCode, headerString, response->content);
    } else {
        payload = fstring(  "HTTP/1.1 %s\r\n" 
                            "%s"
                            "\r\n", statusCode, headerString);
    }

    int result = send(clientSocket, payload, strlen(payload), 0);
    
    hfree(payload);
    
    return result;
}


char* getStatusCodeAsStr(StatusCode code) { 
    char* text;

    switch(code) {
        case OK:
            text = "OK";
            break;
        case NOT_FOUND:
            text = "Not Found";
            break;
        case INTERNAL_SERVER_ERROR:
            text = "Internal Server Error";
            break;
        default: 
            text = "";
            log_error("Error code %d is not handled.", code);
            break;
    }

    return fstring("%d %s", code, text);
}
