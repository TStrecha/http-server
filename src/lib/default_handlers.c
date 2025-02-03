#include "default_handlers.h"
#include "lib/lib.h"

void handle404(Request* request, Response* response) {
    response->statusCode = NOT_FOUND;
    response->contentType = CT_APP_JSON;
    response->content = fstring("{\"error\": 404, \"errorMessage\": \"Resource '%s' coudn't be found on the server.\"}", request->req_line->path);
}

void handle500(Request* request, Response* response) {
    response->statusCode = INTERNAL_SERVER_ERROR;
    response->contentType = CT_APP_JSON;
    response->content = "{\"error\": 500, \"errorMessage\": \"Server couldn't handle your request.\"}";
}
