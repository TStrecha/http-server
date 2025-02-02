#include "error.h"

char* last_err_msg = NO_ERR_MSG;

void setLastErrMsg(char* msg) {
    last_err_msg = msg;
}

void clearLastErrMsg() {
    last_err_msg = NO_ERR_MSG;
}