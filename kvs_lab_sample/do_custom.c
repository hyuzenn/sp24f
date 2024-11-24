#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAPSHOT_PATH "./kvs.img"
#define BUFFER_SIZE 8192  // 버퍼 크기 최적화

// 스냅샷 저장 함수
int do_snapshot(kvs_t* kvs) {
    const char* filepath = SNAPSHOT_PATH;
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to create snapshot file");
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    char buffer[BUFFER_SIZE];
    size_t buffer_len = 0;

    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            // snprintf로 버퍼에 데이터 추가
            int len = snprintf(buffer + buffer_len, sizeof(buffer) - buffer_len, "%s,%s\n", key, value);
            buffer_len += len;

            // 버퍼가 꽉 차면 데이터를 한 번에 파일에 기록
            if (buffer_len >= sizeof(buffer) - 128) {
                ssize_t written = write(fd, buffer, buffer_len);
                if (written == -1) {
                    perror("Write failed");
                    close(fd);
                    return -1;
                }
                buffer_len = 0;
            }
        }
    }
    kvs_iterator_close(it);

    // 남은 데이터 쓰기
    if (buffer_len > 0) {
        ssize_t written = write(fd, buffer, buffer_len);
        if (written == -1) {
            perror("Write failed");
            close(fd);
            return -1;
        }
    }

    // fsync 호출로 디스크에 강제 저장
    if (fsync(fd) == -1) {
        perror("fsync failed");
        close(fd);
        return -1;
    }

    close(fd);
    printf("Snapshot created successfully at %s\n", filepath);
    return 0;
}

// 복구 함수
int do_recovery(kvs_t* kvs) {
    int file = open(SNAPSHOT_PATH, O_RDONLY);
    if (file == -1) {
        perror("Snapshot file not found");
        return -1;
    }

    off_t file_size = lseek(file, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek failed");
        close(file);
        return -1;
    }
    lseek(file, 0, SEEK_SET);

    char* binary_data = (char*)malloc(file_size + 1);
    if (!binary_data) {
        perror("Memory allocation failed");
        close(file);
        return -1;
    }

    ssize_t bytes_read = read(file, binary_data, file_size);
    if (bytes_read != file_size) {
        perror("File read failed");
        free(binary_data);
        close(file);
        return -1;
    }
    binary_data[file_size] = '\0';

    close(file);

    char* line = strtok(binary_data, "\n");
    while (line != NULL) {
        char* comma_pos = strchr(line, ',');
        if (comma_pos != NULL) {
            *comma_pos = '\0';
            char* key = line;
            char* value = comma_pos + 1;

            // 유효한 키와 값만 처리
            int valid = 1;
            for (int i = 0; key[i] != '\0'; i++) {
                if ((unsigned char)key[i] < 32 || (unsigned char)key[i] > 126) {
                    valid = 0;
                    break;
                }
            }
            for (int i = 0; value[i] != '\0'; i++) {
                if ((unsigned char)value[i] < 32 || (unsigned char)value[i] > 126) {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                put(kvs, key, value);
            } else {
                printf("Skipped invalid key-value pair: %s,%s\n", key, value);
            }
        }

        line = strtok(NULL, "\n");
    }

    free(binary_data);

    int answerFile = open("answer.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (answerFile == -1) {
        perror("Failed to open answer.dat");
        return -1;
    }

    struct node* current = kvs->header;
    while (current != NULL) {
        int valid = 1;
        for (int i = 0; current->key[i] != '\0'; i++) {
            if ((unsigned char)current->key[i] < 32 || (unsigned char)current->key[i] > 126) {
                valid = 0;
                break;
            }
        }

        if (current->value != NULL) {
            for (int i = 0; current->value[i] != '\0'; i++) {
                if ((unsigned char)current->value[i] < 32 || (unsigned char)current->value[i] > 126) {
                    valid = 0;
                    break;
                }
            }
        }

        if (valid) {
            ssize_t bytes_written = write(answerFile, current->key, strlen(current->key));
            if (bytes_written == -1) {
                perror("Write failed");
                close(answerFile);
                return -1;
            }
            write(answerFile, ",", 1);
            bytes_written = write(answerFile, current->value, strlen(current->value));
            if (bytes_written == -1) {
                perror("Write failed");
                close(answerFile);
                return -1;
            }
            write(answerFile, "\n", 1);
        }

        current = current->next[0];
    }

    // fsync 호출
    if (fsync(answerFile) == -1) {
        perror("fsync failed");
        close(answerFile);
        return -1;
    }

    close(answerFile);
    return 0;
}
