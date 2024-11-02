    #include "kvs.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    char* get(kvs_t* kvs, const char* key) {
        // 키가 NULL인지 확인
        if (key == NULL) {
            printf("Error: Key is NULL. \n");
            return NULL;
        }

        // Skiplist가 초기화되었는지 확인
        if (kvs == NULL || kvs->list == NULL || kvs->list->header == NULL) {
            printf("Error: Skiplist is not initialized properly. \n");
            return NULL;
        }

        node_t *current = kvs->list->header;

        // 현재 레벨에서 시작하여 키를 찾음
        for (int i = kvs->list->level; i >= 0; i--) {
            // 현재 노드의 forward 포인터를 따라 이동하면서 키를 비교
            while (1) { // 무한 루프를 사용하여 조건을 반복 검사
                if (current->forward[i] == NULL || strcmp(current->forward[i]->key, key) >= 0) {
                    break; // 다음 노드가 없거나 현재 노드의 키가 찾고자 하는 키보다 크거나 같으면 중단
                }
                current = current->forward[i]; // 다음 노드로 이동
            }
        }

        // 가장 아래 레벨에서 키를 확인
        current = current->forward[0];

        // 키가 존재하면 해당 값을 반환
        if (current != NULL && strcmp(current->key, key) == 0) {
            return current->value; // 값 반환
        }

        // 키가 존재하지 않으면 NULL 반환
        return NULL; // 키가 존재하지 않음
    }
