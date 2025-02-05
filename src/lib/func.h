#ifndef _FUNC_H_
#define _FUNC_H_


#define fn(ret_type, body) \
    ({ \
    ret_type __fn__ body\
    __fn__;\
    })

    
#define void_fn(body) \
    ({ \
    void __fn__ body\
    __fn__;\
    })

#define void_ptr_fn(body) \
    ({ \
    void* __fn__ body\
    __fn__;\
    })

#endif