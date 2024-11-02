#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>

int kvs_close(kvs_t* kvs) // 수정된 부분
{
    if (kvs == NULL)
        return -1;
    node_t *current = kvs->list->header;
    while (current) {
        node_t *next = current->forward[0];
        free(current->value);
        current->value = NULL;
        free(current->forward);
        free(current); 
        current = next;
    }
    free(kvs);
    kvs = NULL; // 포인터를 NULL로 설정해도 상위 변수에는 영향을 미치지 않음
    return 0;
}
