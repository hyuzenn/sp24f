#include "kvs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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

    // 파일 크기 구하기
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 파일 데이터를 저장할 버퍼 할당
    char* binary_data = (char*)malloc(file_size + 1);  // +1은 NULL 문자 추가 용
    if (!binary_data) {
        printf("Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    // 파일 읽기
    fread(binary_data, sizeof(char), file_size, file);
    binary_data[file_size] = '\0';  // 끝에 NULL 문자 추가하여 문자열로 만들기

    fclose(file);

    // 바이너리 데이터를 UTF-8로 처리 (C에서는 문자열로 처리됨)
    char* line = strtok(binary_data, "\n");  // 줄 단위로 분리
    while (line != NULL) {
        // 트윗 ID와 데이터가 ','로 구분되어 있으면 분리
        char* comma_pos = strchr(line, ',');
        if (comma_pos != NULL) {
            *comma_pos = '\0';  // ',' 위치에 NULL 문자를 넣어 두 부분으로 나눔
            char* key = line;
            char* value = comma_pos + 1;  // ',' 이후의 데이터

            // 유효성 검사: 키와 값에 유효하지 않은 문자가 포함되어 있으면 건너뛰기
            int valid = 1;
            for (int i = 0; key[i] != '\0'; i++) {
                if ((unsigned char)key[i] < 32 || (unsigned char)key[i] > 126) {
                    valid = 0;  // 유효하지 않은 문자가 포함됨
                    break;
                }
            }
            for (int i = 0; value[i] != '\0'; i++) {
                if ((unsigned char)value[i] < 32 || (unsigned char)value[i] > 126) {
                    valid = 0;  // 유효하지 않은 문자가 포함됨
                    break;
                }
            }

            // 유효한 키와 값만 put 함수에 전달
            if (valid) {
                put(kvs, key, value);
            } else {
                printf("Skipped invalid key-value pair: %s,%s\n", key, value);
            }
        }

        line = strtok(NULL, "\n");  // 다음 줄로 이동
    }

    free(binary_data);  // 할당한 메모리 해제
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
        // 키와 값이 유효한지 체크하고 파일에 기록
        int valid = 1;
        for (int i = 0; current->key[i] != '\0'; i++) {
            if ((unsigned char)current->key[i] < 32 || (unsigned char)current->key[i] > 126) {
                valid = 0;  // 유효하지 않은 문자가 포함됨
                break;
            }
        }
        for (int i = 0; current->value[i] != '\0'; i++) {
            if ((unsigned char)current->value[i] < 32 || (unsigned char)current->value[i] > 126) {
                valid = 0;  // 유효하지 않은 문자가 포함됨
                break;
            }
        }

        // 유효한 데이터만 파일에 기록
        if (valid) {
            fprintf(answerFile, "%s,%s\n", current->key, current->value);
        } else {
            printf("Skipped invalid key-value pair: %s,%s\n", current->key, current->value);
        }

        current = current->next[0];  // 레벨 0으로 이동
    }

    fclose(answerFile);
    return 0;
}
