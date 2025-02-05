#ifndef _OPTION_H_
#define _OPTION_H_

typedef struct {
    void* value;
} Option;

Option option_of(void* value);
void* get(Option* option);
bool has_value(Option* option);
void map(Option* option, void* (*mapper) (void* val));
void if_present(Option* option, void (*runner) (void* val));
void if_present_else(Option* option, void (*runner) (void* val), void (*or_else_runner) ());

#endif