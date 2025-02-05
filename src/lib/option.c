#include <stdbool.h>
#include <stdio.h>
#include "option.h"

Option option_of(void* value) {
    Option opt = {
        .value = value,
    };
    return opt;
}

void* get(Option* option) {
    return option->value;
}

bool has_value(Option* option) {
    return option->value != NULL;
}

void map(Option* option, void* (*mapper) (void* val)) {
    if(option->value != NULL) {
        option->value = mapper(option->value);
    }
}

void if_present(Option* option, void (*runner) (void* val)) {
    if(option->value != NULL) {
        runner(option->value);
    }
}

void if_present_else(Option* option, void (*runner) (void* val), void (*or_else_runner) ()) {
    if(option->value != NULL) {
        runner(option->value);
    } else {
        or_else_runner();
    }
}