#include "kvs.h"

kvs_iterator_t* kvs_iterator(kvs_t* kvs) {
    kvs_iterator_t* it = (kvs_iterator_t*)malloc(sizeof(kvs_iterator_t));
    it->current = kvs->header->next[0]; // Skip List의 첫 번째 노드로 초기화
    return it;
}

int kvs_has_next(kvs_iterator_t* it) {
    return it->current != NULL; // 다음 노드가 존재하는지 확인
}

char* kvs_next_key(kvs_iterator_t* it) {
    if (!it->current) return NULL;

    char* key = it->current->key;    // 현재 노드의 키를 가져옴
    it->current = it->current->next[0]; // 다음 노드로 이동
    return key;
}

void kvs_iterator_close(kvs_iterator_t* it) {
    free(it); // 이터레이터 메모리 해제
}
