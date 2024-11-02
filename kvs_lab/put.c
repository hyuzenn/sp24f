#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int randomLevel(); 

int put(kvs_t* kvs, const char* key, const char* value) {
    if (kvs == NULL || key == NULL || value == NULL) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return -1; // 잘못된 매개변수 처리
    }

    node_t *current = kvs->list->header;
    node_t *update[MAX_LEVEL + 1];
    memset(update, 0, sizeof(node_t*) * (MAX_LEVEL + 1));

    // 노드 탐색
    for (int i = kvs->list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    // 이미 존재하는 키일 경우, 값 업데이트
    if (current != NULL && strcmp(current->key, key) == 0) {
        free(current->value);
        current->value = strdup(value);
        if (current->value == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            return -1; // 메모리 할당 실패 처리
        }
        return 1; // 값 업데이트 성공
    }

    // 새 노드의 레벨 생성
    int lvl = randomLevel(); // 새로운 노드의 레벨을 생성하는 함수 사용
    if (lvl > kvs->list->level) {
        for (int i = kvs->list->level + 1; i <= lvl; i++) {
            update[i] = kvs->list->header;
        }
        kvs->list->level = lvl;
    }

    node_t* newnode = createNode(lvl, key, value);
    if (newnode == NULL) {
        return -1; // 노드 생성 실패 처리
    }

    // 새로운 노드의 forward 포인터 설정
    for (int i = 0; i <= lvl; i++) {
        newnode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newnode;
    }
    return 0; // 새로운 노드 추가 성공
}

// 새로운 레벨을 생성하는 함수
int randomLevel() {
    int lvl = 0;
    while (lvl < MAX_LEVEL && (rand() % 2) == 0) { // 50% 확률로 레벨 증가
        lvl++;
    }
    return lvl;
}
