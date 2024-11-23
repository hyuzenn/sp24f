#include "kvs.h"
#include <stdio.h>
#include <string.h>

int main() {
    kvs_t* kvs = kvs_open();

    if (!kvs) {
        printf("Failed to open kvs\n");
        return -1;
    }

    FILE* trcFile = fopen("cluster004.trc", "r");  // trc 파일을 직접 엽니다.
    FILE* answerFile = fopen("answer.dat", "w");

    if (!trcFile || !answerFile) {
        printf("File not Found\n");
        kvs_close(kvs);
        return -1;
    }
    printf("Opening Files Successed\n");

    char op[10], key_q[100], value_q[100]; // buffer

    while (fscanf(trcFile, "%s %s %s\n", op, key_q, value_q) != EOF) {
        if (strcmp(op, "get") == 0) {
            // get으로 찾은 결과(value)를 answer.dat에 저장, 존재하지 않는 key면 -1로 저장.
            char* return_val = get(kvs, key_q);
            if (!return_val) {
                fprintf(answerFile, "%s\n", "-1");
                continue;
            }
            fprintf(answerFile, "%s\n", return_val);
        } else if (strcmp(op, "set") == 0) {
            // set이면 새로운 key, value 등록
            put(kvs, key_q, value_q);
        }
    }

    fclose(trcFile);
    fclose(answerFile);

    // 스냅샷을 생성하여 현재 상태를 저장
    if (do_snapshot(kvs) == 0) {
        printf("Snapshot saved successfully.\n");
    }

    // KVS 종료
    kvs_close(kvs);

    return 0;
}
