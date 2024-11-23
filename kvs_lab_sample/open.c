#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rand_lv() { // 레벨 난수 생성 함수
    srand(time(NULL));
    int kvs_mx_level = 0;
    while (rand() < RAND_MAX / 2 && kvs_mx_level < MAX_LEVEL - 1) {
        kvs_mx_level++;
    }
    return kvs_mx_level;
}

kvs_t* kvs_open() {
    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    if (!kvs) {
        printf("Allocating kvs Failed\n");
        return NULL;
    }
    kvs->kvs_mx_level = 0;
    kvs->items = 0;

    kvs->header = (node_t*)malloc(sizeof(node_t));
    if (!kvs->header) {
        printf("Allocating header failed\n");
        free(kvs);
        return NULL;
    }
    for (int i = 0; i < MAX_LEVEL; i++) {
        kvs->header->next[i] = NULL;
    }

    // Recovery 호출
    if (do_recovery(kvs) == 0) {
        printf("Recovery successful. kvs has %d items\n", kvs->items);
    } else {
        printf("No snapshot found. Starting with an empty KVS.\n");
    }

    return kvs;
}
