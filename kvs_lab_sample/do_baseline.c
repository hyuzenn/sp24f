#include "kvs.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SNAPSHOT_PATH "./kvs.img" 

int do_snapshot(kvs_t* kvs) {
    int fd = open(SNAPSHOT_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create snapshot file");
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            dprintf(fd, "%s,%s\n", key, value);  // 시스템 호출로 직접 출력
        }
    }
    kvs_iterator_close(it);

    if (fsync(fd) == -1) {
        perror("Failed to sync file to disk");
        close(fd);
        return -1;
    }

    close(fd);
    printf("Snapshot created successfully at %s\n", SNAPSHOT_PATH);
    return 0;
}

int do_recovery(kvs_t* kvs) {
    int fd = open(SNAPSHOT_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Snapshot file not found");
        return -1;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET);

    char* binary_data = (char*)malloc(file_size + 1);
    if (!binary_data) {
        perror("Memory allocation failed");
        close(fd);
        return -1;
    }

    ssize_t bytes_read = read(fd, binary_data, file_size);
    if (bytes_read != file_size) {
        perror("File read failed");
        free(binary_data);
        close(fd);
        return -1;
    }
    binary_data[file_size] = '\0';
    close(fd);

    char* line = strtok(binary_data, "\n");
    while (line != NULL) {
        // 개행 문자가 있다면 제거
        char* line_end = strchr(line, '\n');
        if (line_end != NULL) {
            *line_end = '\0';  // 개행 문자 제거
        }

        char* comma_pos = strchr(line, ',');
        if (comma_pos != NULL) {
            *comma_pos = '\0';  // key와 value 분리
            char* key = line;
            char* value = comma_pos + 1;

            // NULL 문자 설정
            key[strcspn(key, "\r\n")] = '\0';
            value[strcspn(value, "\r\n")] = '\0';

            put(kvs, key, value);  // 유효성 검사 제거
        }
        line = strtok(NULL, "\n");
    }
    free(binary_data);
    printf("Recovery completed successfully from %s\n", SNAPSHOT_PATH);
    return 0;
}
