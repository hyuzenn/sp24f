#include "kvs.h"

int rand_lv(){ //레벨 난수 생성 함수
	srand(time(NULL));
    int kvs_mx_level = 0;
    while (rand() < RAND_MAX / 2 && kvs_mx_level < MAX_LEVEL - 1) {
        kvs_mx_level++;
    }
    return kvs_mx_level;
}

kvs_t* open()
{
	kvs_t* kvs = (kvs_t*) malloc (sizeof(kvs_t));
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

	printf("Open: kvs has %d items\n", kvs->items);


	return kvs;
}
