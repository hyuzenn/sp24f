#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>

kvs_t* open_kvs() {
    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    
    if (!kvs) {
        printf("Error: Failed to allocate memory for kvs\n");
        return NULL;
    }

    kvs->list = createSkiplist();
    if (!kvs->list) {
        free(kvs);
        printf("Error: Failed to create skiplist\n");
        return NULL;
    }

    return kvs;
}