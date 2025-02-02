#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "lib/mem.h"

unsigned long strhashcode(char* str);

StrStrHashMap* init_sshmap() {
    StrStrHashMap* out = (StrStrHashMap*) hmalloc(sizeof(StrStrHashMap));
    if(out == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < MAP_SIZE; i++) {
        out->buckets[i] = NULL;
    }

    out->size = MAP_SIZE;
    out->elements = 0;

    return out;
}

int insert_sshmap(StrStrHashMap* self, char* key, char* value) {
    unsigned long hash = strhashcode(key) % self->size;
    
    StrStrNode* new_node = (StrStrNode*) hmalloc(sizeof(StrStrNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;

    StrStrNode* bucket_end = self->buckets[hash];

    if(bucket_end == NULL) {
        self->buckets[hash] = new_node;
    } else {
        while(bucket_end->next != NULL) {
            bucket_end = bucket_end->next;
        }

        bucket_end->next = new_node;
    }

    self->elements++;

    return 0;
}

char* get_sshmap(StrStrHashMap* self, char* key) {
    unsigned long hashcode = strhashcode(key);
    long hash = hashcode % self->size;
    StrStrNode* bucket_end = self->buckets[hash];

    if(bucket_end == NULL) {
        return NULL;
    }
    
    do {
        if(strhashcode(bucket_end->key) == hashcode) {
            return bucket_end->value;
        }

        bucket_end = bucket_end->next;
    } while(bucket_end != NULL);

    return NULL;
}

StrStrNode** get_all_sshmap(StrStrHashMap* self) {
    StrStrNode** elements = (StrStrNode**) hmalloc(self->elements * sizeof(StrStrNode*));
    int elementCount = 0;

    for(int i = 0; i < self->size; i++) {
        StrStrNode* node = self->buckets[i];

        while(node != NULL) {
            elements[elementCount] = node;

            node = node->next;
            elementCount++;
        }
    }

    return elements;
}


// From https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned long strhashcode(char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}