#ifndef _ERROR_H_
#define _ERROR_H_

#define NOERR                   0
#define MEMALLOCERR             -1

#define REQERR                  1000
#define REQ_INVMETHODERR        -(REQERR + 1)
#define REQ_INVPRTCLERR         -(REQERR + 2)
#define REQ_INVREQLINE          -(REQERR + 3)
#define REQ_HEADERTOOBIG        -(REQERR + 4)
#define REQ_MALFORMEDREQLINE    -(REQERR + 5)

#define NO_ERR_MSG "No Error Message Available"

extern char* last_err_msg;
void setLastErrMsg(char* msg);
void clearLastErrMsg();

#define log_wsa_err(msg) log_error("(WSA%d) %s", WSAGetLastError(), msg)

#define fnErrMsg(ftm, ...) setLastErrMsg(fstring("%s:%d#%s(): %s", __FILE__, __LINE__, __func__, fstring(ftm, ##__VA_ARGS__)))

#define check_res(x) if(x != NOERR) { log_error("App error(%d): %s", x, last_err_msg); clearLastErrMsg(); }
#define check_res_ret(x) if(x != NOERR) { check_res(x); return x; }
#define check_res_ret_ptr(x) if(x != NOERR) { check_res(x); return NULL; }

#endif