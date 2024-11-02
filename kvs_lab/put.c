#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int randomLevel(); 

int put(kvs_t* kvs, const char* key, const char* value) {
    if (kvs == NULL || key == NULL || value == NULL) {
        printf("Error: Invalid parameters. \n");
        return -1; // 잘못된 매개변수 처리
    }

    node_t *current = kvs->list->header;
    node_t *update[MAX_LEVEL + 1];
    for (int i = 0; i <= MAX_LEVEL; i++) {
        update[i] = 0; 
    }

    // 노드 탐색
    for (int i = kvs->list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i]; // 다음 노드로 이동
        }
        update[i] = current; // 현재 노드를 업데이트
    }

    // 다음 노드를 찾기
    current = (current->forward[0] != NULL && strcmp(current->forward[0]->key, key) == 0) ? current->forward[0] : NULL;

    // 이미 존재하는 키일 경우, 값 업데이트
    if (current) { // 키가 일치하는 경우
        if (strcmp(current->key, key) == 0) {
            // 기존 값 해제 및 새로운 값 할당
            char *new_value = strdup(value);
            if (new_value == NULL) {
                printf("Error: Memory allocation failed. \n");
                return -1; // 메모리 할당 실패 처리
            }
            // 기존 값 해제 후 새 값으로 교체
            free(current->value);
            current->value = new_value;
            return 1; // 값 업데이트 성공
        }
    }

    // 새 노드의 레벨 생성
    int lvl = randomLevel(); // 새로운 노드의 레벨을 생성하는 함수 사용
    int new_levels = lvl - kvs->list->level; // 증가된 레벨 수

    // Skip List의 레벨을 갱신
    if (lvl > kvs->list->level) {
        memset(update + kvs->list->level + 1, 0, sizeof(node_t*) * new_levels); // 헤더 노드로 초기화
        kvs->list->level = lvl; // 레벨 갱신
    }

    node_t* newnode = createNode(lvl, key, value);
    if (newnode == NULL) {
        return -1; // 노드 생성 실패 처리
    }


    for (int i = 0; i <= lvl; i++) {
        // update[i]가 NULL이 아닌지 확인
        if (update[i] != NULL) {
            node_t* temp = update[i]->forward[i]; // 현재 forward 포인터 저장
            update[i]->forward[i] = newnode; // 새로운 노드를 현재 노드의 forward 포인터에 연결
            newnode->forward[i] = temp; // 저장한 forward 포인터를 새로운 노드의 forward에 연결
        }
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