#include <stdio.h>
#include "kvs.h"

kvs_t* open() {
		kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
	if (kvs == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			return NULL; // 메모리 할당 실패 시 NULL 반환
	}

    kvs->header = create_node(MAX_LEVEL, "", ""); // 헤더 노드 생성
    kvs->level = 0;
    kvs->items = 0;
    srand(time(NULL)); // 랜덤 시드 초기화

    printf("Open: kvs has %d items\n", kvs->items);
    fflush(stdout); // 출력 버퍼를 비웁니다.

    return kvs;
}
