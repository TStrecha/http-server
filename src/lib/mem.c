#include <stdio.h>
#include <stdlib.h>
#include "log/log.h"
#include "mem.h"

void* handled_malloc(int size, char* file, int line, const char* func) {
    void* ptr = malloc(size);

    log_debug("%s:%d#%s() allocated %d bytes of memory at 0x%p", file, line, func, size, ptr);

    return ptr;
}


void* handled_free(void* ptr, char* file, int line, const char* func) {
    free(ptr);

    log_debug("%s:%d#%s() freed memory address at 0x%p", file, line, func, ptr);
}