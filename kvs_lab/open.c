#include "kvs.h"

kvs_t* open_kvs() {
    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    kvs->header = create_node(MAX_LEVEL, "", ""); // 헤더 노드 생성
    kvs->level = 0;
    kvs->items = 0;
    srand(time(NULL)); // 랜덤 시드 초기화
    return kvs;
}