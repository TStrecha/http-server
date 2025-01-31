#include <winsock2.h>
#include <stdio.h>

#include "lib.h"

int isNumber(char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char) str[i]) && str[i] != '-') {
            return 0;
        }
    }
    return 1;
}

char* fstring(char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    int size = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* formatted_str = (char*) malloc(size + 1);

    va_start(args, fmt);
    vsnprintf(formatted_str, size + 1, fmt, args);
    va_end(args);

    return formatted_str;
}