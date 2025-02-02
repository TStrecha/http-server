#ifndef _MEM_H_
#define _MEM_H_

void* handled_malloc(int size, char* file, int line, const char* func);
void* handled_free(void* ptr, char* file, int line, const char* func);

#ifdef MEMDEB
    #define hmalloc(size) handled_malloc(size, __FILE__, __LINE__, __func__)
    #define hfree(ptr) handled_free(ptr, __FILE__, __LINE__, __func__)
#else
    #define hmalloc(size) malloc(size)
    #define hfree(ptr) free(ptr)
#endif

#endif
