#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAPSHOT_PATH "./kvs.img"
#define MAX_BUFFER_SIZE 8192  // 버퍼 크기 설정

int do_snapshot(kvs_t* kvs) {
    int fd = open(SNAPSHOT_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to create snapshot file");
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    char buffer[MAX_BUFFER_SIZE];
    ssize_t written_size;

    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            // Write the key-value pair into the buffer
            int len = snprintf(buffer, MAX_BUFFER_SIZE, "%s,%s\n", key, value);
            if (len < 0) {
                perror("Failed to format key-value pair");
                kvs_iterator_close(it);
                close(fd);
                return -1;
            }

            // Write to the file
            written_size = write(fd, buffer, len);
            if (written_size < len) {
                perror("Failed to write key-value pair");
                kvs_iterator_close(it);
                close(fd);
                return -1;
            }
        }
    }
    kvs_iterator_close(it);

    if (fsync(fd) == -1) {
        perror("fsync failed");
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

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read;
    char* line;

    while ((bytes_read = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer
        line = strtok(buffer, "\n");
        while (line != NULL) {
            char* comma_pos = strchr(line, ',');
            if (comma_pos != NULL) {
                *comma_pos = '\0';
                char* key = line;
                char* value = comma_pos + 1;

                // NULL 문자 설정
                key[strcspn(key, "\r\n")] = '\0';
                value[strcspn(value, "\r\n")] = '\0';

                put(kvs, key, value);  // 유효성 검사 제거
            }
            line = strtok(NULL, "\n");
        }
    }

    if (bytes_read < 0) {
        perror("File read failed");
        close(fd);
        return -1;
    }

    close(fd);
    printf("Recovery completed successfully from %s\n", SNAPSHOT_PATH);
    return 0;
}
