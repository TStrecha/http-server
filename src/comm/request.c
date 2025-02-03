#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#include "request.h"
#include "log/log.h"
#include "lib/lib.h"
#include "lib/map.h"
#include "lib/error.h"
#include "lib/mem.h"

StrHashMap* extractHeaders(char* headers);

int extractHeaderFromPayload(char** out, char* payload);
int extractBodyFromPayload(char** out, char* payload, int headerSize);
int extractRequestLineFromHeader(char** out, char* header);
int parseRequestLine(RequestLine* out, RequestLineRaw* rlRaw);
int parseMethod(RequestMethod* method, char* methodPart);
int parseProtocol(Protocol* protocol, char* pvPart);

Request* readRequest(SOCKET clientSocket) {
    char* recvbuffer = (char*) hmalloc(READ_BUFFER_SIZE + 1);
    if(recvbuffer == NULL) {
        return NULL;
    }

    int payload_size = recv(clientSocket, recvbuffer, READ_BUFFER_SIZE, 0);
    if(payload_size <= 0) {
        log_wsa_err("Recv failed.");

        hfree(recvbuffer);
        return NULL;
    }

    int res;

    char* header;
    res = extractHeaderFromPayload(&header, recvbuffer);
    check_res_ret_ptr(res);

    char* body;
    res = extractBodyFromPayload(&body, recvbuffer, strlen(header));
    check_res_ret_ptr(res);

    char* req_line;
    res = extractRequestLineFromHeader(&req_line, header);
    check_res_ret_ptr(res);

    RequestLineRaw rlRaw = {0};
    parseRequestLineRaw(&rlRaw, req_line);
    log_info("(CL%d) Got request %s %s through %s", clientSocket, rlRaw.method, rlRaw.path, rlRaw.protocol);

    RequestLine* rl = (RequestLine*) hmalloc(sizeof(RequestLine));
    res = parseRequestLine(rl, &rlRaw);
    check_res_ret_ptr(res);

    StrHashMap* headers = extractHeaders(header + strlen(req_line));

    Request* request = (Request*) hmalloc(sizeof(Request));
    if(request == NULL) {
        return NULL;
    }

    request->req_line = rl;
    request->headers = headers;
    request->body = body;

    hfree(recvbuffer);

    return request;
}


int extractHeaderFromPayload(char** out, char* payload) {
    char* delimiter = strstr(payload, "\r\n\r\n");
    int buff_size = delimiter - payload;
    
    char* buff = (char*) hmalloc(buff_size + 1);
    if(buff == NULL) {
        fnErrMsg("Memory allocation error.");
        return MEMALLOCERR;
    }

    strncpy(buff, payload, buff_size);
    buff[buff_size] = '\0';

    *out = buff;

    return NOERR;
}

int extractBodyFromPayload(char** out, char* payload, int headerSize) {
    if(headerSize >= strlen(payload)) {
        return REQ_HEADERTOOBIG;
    }

    int buff_size = strlen(payload) - headerSize;
    char* buff = (char*) hmalloc(buff_size + 1);
    if(buff == NULL) {
        fnErrMsg("Memory allocation error.");
        return MEMALLOCERR;
    }

    strncpy(buff, payload + headerSize + 4, buff_size);
    buff[buff_size] = '\0';

    *out = buff;

    return NOERR;
}

int extractRequestLineFromHeader(char** out, char* header) {
    char* requestLineEndLoc = strstr(header, "\r\n");
    if(requestLineEndLoc == NULL) {
        fnErrMsg("Request line couldn't be found in request header.");
        return REQ_INVREQLINE;
    }

    int requestLineSize = requestLineEndLoc - header;
    char* requestLine = (char*) hmalloc(requestLineSize + 1);
    if(requestLine == NULL) {
        fnErrMsg("Memory allocation error.");
        return MEMALLOCERR;
    }

    strncpy(requestLine, header, requestLineSize);
    requestLine[requestLineSize] = '\0';

    *out = requestLine;

    return NOERR;
}


int parseRequestLineRaw(RequestLineRaw* out, char* requestLine) {
    char* method = (char*) hmalloc(10);
    char* path = (char*) hmalloc(1024);
    char* prot_ver = (char*) hmalloc(16);

    if(sscanf(requestLine, "%9s %1023s %15s", method, path, prot_ver) < 3) {
        return REQ_MALFORMEDREQLINE;
    }
    
    out->method = method;
    out->path = path;
    out->protocol = prot_ver;
}

int parseRequestLine(RequestLine* out, RequestLineRaw* rlRaw) {
    RequestMethod method;
    int err = parseMethod(&method, rlRaw->method);
    if(err != NOERR) {
        return err;
    }

    Protocol protocol;
    err = parseProtocol(&protocol, rlRaw->protocol);
    if(err != NOERR) {
        return err;
    }

    out->path = hmalloc(strlen(rlRaw->path) + 1);
    if(out->path == NULL) {
        fnErrMsg("Memory allocation error.");
        return MEMALLOCERR;
    }
    strcpy(out->path, rlRaw->path);
    out->method = method;
    out->protocol = protocol;

    return NOERR;
}

StrHashMap* extractHeaders(char* headers) {
    char* token = strtok(headers, "\r\n");

    StrHashMap* header_map = init_shmap();
    if(header_map == NULL) {
        return NULL;
    }

    while(token != NULL) {
        char* separatorLoc = strstr(token, ": ");

        int keyLength = separatorLoc - token;
        int valueLength = strlen(token) - keyLength - 2;

        char* key = (char*) hmalloc(keyLength + 1);
        if(key == NULL) {
            return NULL;
        }
        strncpy(key, token, keyLength);
        key[keyLength] = '\0';

        char* value = (char*) hmalloc(valueLength + 1);
        if(value == NULL) {
            return NULL;
        }
        strncpy(value, token + keyLength + 2, valueLength);
        value[valueLength] = '\0';
        
        insert_shmap(header_map, key, value);

        token = strtok(NULL, "\r\n");
    }

    return header_map;
}

int parseMethod(RequestMethod* method, char* methodPart) {
    if(strcmp(methodPart, "GET") == 0) {
        *method = GET;
        return NOERR;
    } else if(strcmp(methodPart, "POST") == 0) {
        *method = POST;
        return NOERR;
    }

    fnErrMsg(fstring("Invalid method %s", methodPart));
    return REQ_INVMETHODERR;
}

int parseProtocol(Protocol* protocol, char* pvPart) {
    if(strcmp(pvPart, "HTTP/1.1") == 0) {
        *protocol = HTTP_1_1;
        return NOERR;
    }

    fnErrMsg(fstring("Invalid protocol %s", pvPart));
    return REQ_INVPRTCLERR;
}

char* getMethodAsStr(RequestMethod method) { 
    switch(method) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        default: 
            log_error("Method %d is not handled.", method);
            return "";
    }
}
