#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "lib/mem.h"

unsigned long strhashcode(char* str);

StrHashMap* init_shmap() {
    StrHashMap* out = (StrHashMap*) hmalloc(sizeof(StrHashMap));
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

int insert_shmap(StrHashMap* self, char* key, void* value) {
    unsigned long hash = strhashcode(key) % self->size;
    
    StrKeyNode* new_node = (StrKeyNode*) hmalloc(sizeof(StrKeyNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;

    StrKeyNode* bucket_end = self->buckets[hash];

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

void* get_shmap(StrHashMap* self, char* key) {
    unsigned long hashcode = strhashcode(key);
    long hash = hashcode % self->size;
    StrKeyNode* bucket_end = self->buckets[hash];

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

StrKeyNode** get_all_shmap(StrHashMap* self) {
    StrKeyNode** elements = (StrKeyNode**) hmalloc(self->elements * sizeof(StrKeyNode*));
    int elementCount = 0;

    for(int i = 0; i < self->size; i++) {
        StrKeyNode* node = self->buckets[i];

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