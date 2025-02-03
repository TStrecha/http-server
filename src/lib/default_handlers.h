#ifndef _DEFAULT_HANDLERS_H_
#define _DEFAULT_HANDLERS_H_

#include "comm/request.h"
#include "comm/response.h"

void handle404(Request* request, Response* response);
void handle500(Request* request, Response* response);

#endif