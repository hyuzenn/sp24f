#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // KVS 열기
    kvs_t* kvs = kvs_open();
    if (!kvs) {
        printf("Failed to open kvs\n");
        return -1;
    }

    // 테스트 워크로드 실행
    FILE* trcFile = fopen("cluster004.trc", "r");
    FILE* answerFile = fopen("answer.dat", "w");

    if (!trcFile || !answerFile) {
        printf("File not Found\n");
        kvs_close(kvs);
        return -1;
    }
    printf("Opening Files Successed\n");

    char op[10], key_q[MAX_KEY_LENGTH], value_q[MAX_VALUE_LENGTH];
    while (fscanf(trcFile, "%99[^,],%999[^,],%9999s\n", op, key_q, value_q) != EOF) {
        if (strcmp(op, "get") == 0) {
            // get으로 찾은 결과를 answer.dat에 저장
            char* return_val = get(kvs, key_q);
            if (!return_val) {
                fprintf(answerFile, "%s\n", "-1");
                continue;
            }
            fprintf(answerFile, "%s\n", return_val);
        } else if (strcmp(op, "set") == 0) {
            // set(get)이면 새로운 key, value 등록
            put(kvs, key_q, value_q);
        }
    }

    fclose(trcFile);
    fclose(answerFile);

    // 스냅샷 저장
    if (do_snapshot(kvs) != 0) {
        printf("Snapshot failed\n");
        kvs_close(kvs);
        return -1;
    }

    printf("Snapshot completed successfully.\n");

    // KVS 종료 후 재시작을 위한 복구
    kvs_close(kvs);
    printf("KVS closed. Starting recovery...\n");

    kvs = kvs_open();
    if (!kvs) {
        printf("Failed to open kvs for recovery\n");
        return -1;
    }

    if (do_recovery(kvs) != 0) {
        printf("Recovery failed\n");
        kvs_close(kvs);
        return -1;
    }

    // 복구 후 특정 키에 대한 get 요청 및 출력
    char* keys[] = {"tweet55", "tweet13843", "tweet3482"};
    for (int i = 0; i < 3; i++) {
        char* value = get(kvs, keys[i]);
        if (value) {
            printf("Key: %s, Value: %s\n", keys[i], value);
        } else {
            printf("Key: %s, Value not found\n", keys[i]);
        }
    }

    // KVS 종료
    kvs_close(kvs);
    printf("Recovery completed successfully and KVS closed.\n");

    return 0;
}
