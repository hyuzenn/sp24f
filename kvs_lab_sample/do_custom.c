#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "kvs.h"

int do_snapshot(kvs_t* kvs) {
    const char* filepath = "./kvs.img";
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Failed to create snapshot file at %s\n", filepath);
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    char buffer[4096];  // 버퍼 크기 최적화
    size_t buffer_len = 0;

    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            int len = snprintf(buffer + buffer_len, sizeof(buffer) - buffer_len, "%s,%s\n", key, value);
            buffer_len += len;

            if (buffer_len >= sizeof(buffer) - 128) {  // 버퍼가 꽉 차면 쓰기
                write(fd, buffer, buffer_len);
                buffer_len = 0;
            }
        }
    }
    kvs_iterator_close(it);

    if (buffer_len > 0) {
        write(fd, buffer, buffer_len);  // 남은 데이터 쓰기
    }

    fsync(fd);
    close(fd);
    printf("Snapshot created successfully at %s\n", filepath);
    return 0;
}

int do_recovery(kvs_t* kvs) {
    // 복구 파일 경로
    const char* filepath = "C:\\Users\\huj22\\source\\repos\\sp24f\\kvs_lab_sample\\kvs.img";

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        printf("Snapshot file not found at %s\n", filepath);
        return -1;
    }

    char buffer[256];
    ssize_t bytesRead;
    char key[100], value[100];
    int i = 0, j = 0;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (i = 0; i < bytesRead; ++i) {
            if (buffer[i] == ',') {
                key[j] = '\0';
                j = 0;
            } else if (buffer[i] == '\n') {
                value[j] = '\0';
                put(kvs, key, value);
                j = 0;
            } else {
                if (j < sizeof(key) - 1) {
                    (buffer[i - 1] == ',') ? (value[j++] = buffer[i]) : (key[j++] = buffer[i]);
                }
            }
        }
    }

    close(fd);
    printf("Recovery completed successfully from %s\n", filepath);
    return 0;
}
