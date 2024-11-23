#include "kvs.h"
#include <stdio.h>
#include <unistd.h>

int do_snapshot(kvs_t* kvs) {
    const char* filepath = "./kvs.img";  // 현재 디렉토리에 저장
    FILE* file = fopen(filepath, "w");
    if (!file) {
        printf("Failed to create snapshot file at %s\n", filepath);
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

    printf("Snapshot created successfully at %s\n", filepath);
    return 0;
}

int do_recovery(kvs_t* kvs) {
    // 복구 파일 경로
    const char* filepath = "C:\\Users\\huj22\\source\\repos\\sp24f\\kvs_lab_sample\\kvs.img";

    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Snapshot file not found at %s\n", filepath);
        return -1;
    }

    char key[100], value[100];
    while (fscanf(file, "%[^,],%s\n", key, value) != EOF) {
        put(kvs, key, value);
    }

    fclose(file);
    printf("Recovery completed successfully from %s\n", filepath);
    return 0;
}
