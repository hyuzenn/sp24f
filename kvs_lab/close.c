#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>


int kvs_close(kvs_t* kvs) {
    if (kvs == NULL) {
        return -1; // NULL 포인터에 대한 체크 추가
    }

    node_t *current = kvs->list->header->forward[0]; // 리스트의 첫 번째 노드에서 시작

    while (current) {
        node_t *next = current->forward[0];
        free(current->value); // 현재 노드의 값 해제
        free(current); // 현재 노드 해제
        current = next; // 다음 노드로 이동
    }

    free(kvs->list->header); // 리스트의 헤더 노드 해제
    free(kvs->list); // 리스트 자체 해제
    free(kvs); // KVS 구조체 해제
    kvs = NULL; // 포인터를 NULL로 설정해도 상위 변수에는 영향을 미치지 않음

    return 0;
}
