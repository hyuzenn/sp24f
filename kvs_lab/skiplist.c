#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t* createNode(int level, const char *key, const char *value) {
    node_t *n = (node_t*) malloc(sizeof(node_t));
    if (n == NULL) {
        printf("Error: Memory allocation for node failed. \n");
        return NULL; // 메모리 할당 실패 처리
    }

    n->key = (char*) malloc(strlen(key) + 1);
    if (n->key == NULL) {
        free(n);
        printf("Error: Memory allocation for key failed. \n");
        return NULL; // 메모리 할당 실패 처리
    }
    strcpy(n->key, key);

    n->value = strdup(value);
    if (n->value == NULL) {
        free(n->key);
        free(n);
        printf("Error: Memory allocation for value failed. \n");
        return NULL; // 메모리 할당 실패 처리
    }

    n->forward = (node_t**) malloc(sizeof(node_t*) * (level + 1));
    if (n->forward == NULL) {
        free(n->key);
        free(n->value);
        free(n);
        printf("Error: Memory allocation for forward pointers failed. \n");
        return NULL; // 메모리 할당 실패 처리
    }

    for (int i = 0; i <= level; i++) {
        n->forward[i] = NULL;
    }
    return n;
}

skiplist_t* createSkiplist() {
    skiplist_t* list = (skiplist_t*)malloc(sizeof(skiplist_t));
    if (!list) {
        printf("Failed to allocate memory for skiplist. \n");
        return NULL;
    }

    // 헤더 노드 메모리 할당
    list->header = (node_t*)malloc(sizeof(node_t));
    if (!list->header) {
        free(list); // 리스트 메모리 해제
        printf("Failed to allocate memory for header node. \n");
        return NULL;
    }

    // 헤더 노드의 forward 배열 메모리 할당
    list->header->forward = (node_t**)malloc(sizeof(node_t*) * (MAX_LEVEL + 1));
    if (!list->header->forward) {
        free(list->header); // 헤더 노드 메모리 해제
        free(list); // 리스트 메모리 해제
        printf("Failed to allocate memory for header's forward pointers. \n");
        return NULL;
    }

    // 헤더 노드 초기화
    for (int i = 0; i <= MAX_LEVEL; i++) {
        list->header->forward[i] = NULL; // 모든 forward 포인터를 NULL로 초기화
    }
    list->header->key = NULL; // 헤더 노드는 키가 필요 없음
    list->header->value = NULL; // 헤더 노드는 값이 필요 없음
    list->level = 0; // 현재 레벨을 0으로 초기화

    printf("Skiplist created successfully. \n"); // 디버깅 출력
    return list;
}
