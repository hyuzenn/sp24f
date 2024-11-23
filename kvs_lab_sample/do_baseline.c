#include "kvs.h"
#include <stdio.h>
#include <unistd.h>
#define SNAPSHOT_PATH "/mnt/c/Users/huj22/source/repos/sp24f/kvs_lab_sample/kvs.img" 

int do_snapshot(kvs_t* kvs) {
    FILE* file = fopen(SNAPSHOT_PATH, "w");
    if (!file) {
        printf("Failed to create snapshot file at %s\n", SNAPSHOT_PATH);
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            fprintf(file, "%s,%s\n", key, value);
        }
    }
    kvs_iterator_close(it);

    fflush(file);  // 버퍼 내용 플러시
    int fd = fileno(file);  // FILE*에서 파일 디스크립터 가져오기
    fsync(fd);  // 디스크에 강제 저장
    fclose(file);

    printf("Snapshot created successfully at %s\n", SNAPSHOT_PATH);
    return 0;
}

node_t* search(node_t* head, const char* key) {
    node_t* current = head;
    int level = MAX_LEVEL - 1;  // 스킵 리스트의 최고 레벨부터 시작

    while (level >= 0) {
        // 현재 레벨에서 key보다 작은 노드로 이동
        while (current->next[level] != NULL && strcmp(current->next[level]->key, key) < 0) {
            current = current->next[level];  // 해당 레벨에서 다음 노드로 이동
        }
        level--;  // 레벨을 하나 낮추기
    }

    // 가장 낮은 레벨에서 최종적으로 key를 가진 노드를 찾음
    return current->next[0];  // key를 찾은 후, 해당 노드 반환
}

int do_recovery(kvs_t* kvs) {
    // 복구 파일 경로
    FILE* file = fopen(SNAPSHOT_PATH, "rb");
    if (!file) {
        printf("Snapshot file not found at %s\n", SNAPSHOT_PATH);
        return -1;
    }

    char key[100], value[100];
    while (fscanf(file, "%[^,],%s\n", key, value) != EOF) {
        // 복구 파일에서 읽은 데이터를 put 함수로 추가
        put(kvs, key, value);
    }

    fclose(file);
    printf("Recovery completed successfully from %s\n", SNAPSHOT_PATH);

    // answer.dat 파일에 복구된 데이터를 저장
    FILE* answerFile = fopen("answer.dat", "w");
    if (!answerFile) {
        printf("Failed to open answer.dat\n");
        return -1;
    }

    // KVS에서 데이터를 읽어 answer.dat에 기록
    struct node* current = kvs->header;  // KVS의 첫 번째 노드부터 시작
    while (current != NULL) {
        fprintf(answerFile, "%s,%s\n", current->key, current->value);  // key, value 쌍을 기록
        int level = 0;  // 예시로 레벨 0부터 시작한다고 가정
        current = current->next[level];  // level 0에서 다음 노드로 이동
    }

    fclose(answerFile);
    return 0;
}
