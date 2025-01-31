#ifndef _MAP_H_
#define _MAP_H_

typedef struct StrStrNode {
    char* key;
    char* value;
    struct StrStrNode* next;
} StrStrNode;

#define MAP_SIZE 10

typedef struct {
    StrStrNode* buckets[MAP_SIZE];
    int size;
    int elements;
} StrStrHashMap;

StrStrHashMap* init_sshmap();
char* get_sshmap(StrStrHashMap* self, char* key);
int insert_sshmap(StrStrHashMap* self, char* key, char* value);
StrStrNode** get_all_sshmap(StrStrHashMap* self);

#endif