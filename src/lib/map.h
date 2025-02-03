#ifndef _MAP_H_
#define _MAP_H_

typedef struct StrKeyNode {
    char* key;
    void* value;
    struct StrKeyNode* next;
} StrKeyNode;

#define MAP_SIZE 10

typedef struct {
    StrKeyNode* buckets[MAP_SIZE];
    int size;
    int elements;
} StrHashMap;

StrHashMap* init_shmap();
void* get_shmap(StrHashMap* self, char* key);
int insert_shmap(StrHashMap* self, char* key, void* value);
StrKeyNode** get_all_shmap(StrHashMap* self);

#endif