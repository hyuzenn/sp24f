#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAPSHOT_PATH "./kvs.img"
#define MAX_BUFFER_SIZE 8192  // 한번에 더 많이 쓰기

int do_snapshot(kvs_t* kvs) {
    const char* filepath = SNAPSHOT_PATH;
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to create snapshot file");
        return -1;
    }

    kvs_iterator_t* it = kvs_iterator(kvs);
    char buffer[MAX_BUFFER_SIZE];  // 최적화된 버퍼 크기
    size_t buffer_len = 0;

    while (kvs_has_next(it)) {
        char* key = kvs_next_key(it);
        char* value = get(kvs, key);
        if (key && value) {
            // snprintf를 이용하여 key와 value를 buffer에 추가
            int len = snprintf(buffer + buffer_len, sizeof(buffer) - buffer_len, "%s,%s\n", key, value);
            buffer_len += len;

            // 버퍼가 꽉 차면 파일에 쓰기
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

    // 한 번만 fsync 호출로 디스크에 강제 저장
    if (fsync(fd) == -1) {
        perror("fsync failed");
        close(fd);
        return -1;
    }
    printf("Data successfully written to disk using fsync.\n");

    // 파일 닫기
    close(fd);
    printf("Snapshot created successfully at %s\n", filepath);
    return 0;
}

int do_recovery(kvs_t* kvs) {
    int file = open(SNAPSHOT_PATH, O_RDONLY);
    if (file == -1) {
        perror("Snapshot file not found");
        return -1;
    }

    // 파일 크기 구하기
    off_t file_size = lseek(file, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek failed");
        close(file);
        return -1;
    }
    lseek(file, 0, SEEK_SET);

    // 파일 데이터를 저장할 버퍼 할당
    char* binary_data = (char*)malloc(file_size + 1);  // +1은 NULL 문자 추가 용
    if (!binary_data) {
        perror("Memory allocation failed");
        close(file);
        return -1;
    }

    // 파일 읽기
    ssize_t bytes_read = read(file, binary_data, file_size);
    if (bytes_read != file_size) {
        perror("File read failed");
        free(binary_data);
        close(file);
        return -1;
    }
    binary_data[file_size] = '\0';  // 끝에 NULL 문자 추가하여 문자열로 만들기

    close(file);

    // 바이너리 데이터를 UTF-8로 처리 (C에서는 문자열로 처리됨)
    char* line = strtok(binary_data, "\n");
    while (line != NULL) {
        char* comma_pos = strchr(line, ',');
        if (comma_pos != NULL) {
            *comma_pos = '\0';  // 키와 값 분리
            char* key = line;
            char* value = comma_pos + 1;

            // 유효성 검사
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

            // 유효한 키와 값만 put 함수에 전달
            if (valid) {
                put(kvs, key, value);
            } else {
                printf("Skipped invalid key-value pair: %s,%s\n", key, value);
            }
        }

        line = strtok(NULL, "\n");
    }

    free(binary_data);
    printf("Recovery completed successfully from %s\n", SNAPSHOT_PATH);

    // answer.dat에 복구된 데이터를 저장
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

        // 유효한 데이터만 파일에 기록
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

    // fsync 한 번만 호출
    if (fsync(answerFile) == -1) {
        perror("fsync failed");
        close(answerFile);
        return -1;
    }
    printf("Data successfully written to disk using fsync.\n");

    close(answerFile);
    return 0;
}
