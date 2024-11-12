#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    int fd;
    char* buff = NULL;  // 초기화
    ssize_t nreads;
    struct stat file_stat;
    size_t buff_size;

    // 인자 확인 및 파일 열기
    if (argc < 2) {
        fprintf(stdout, "No arguments\n");
        return 0;
    } else if (argc == 2) {
        // 표준 출력으로 출력하는 경우
        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Failed to open file %s: %s\n", argv[1], strerror(errno));
            return 1;
        }

        // 파일 크기 확인 및 버퍼 할당
        if (fstat(fd, &file_stat) == -1) {
            fprintf(stderr, "Failed to get file size: %s\n", strerror(errno));
            close(fd);
            return 1;
        }
        buff_size = file_stat.st_size > 0 ? file_stat.st_size : 16;  // 최소 16바이트 확보
        buff = (char*)malloc(buff_size);
        if (!buff) {
            fprintf(stderr, "Failed to allocate memory\n");
            close(fd);
            return 1;
        }

        // 파일 내용 읽어 출력
        while ((nreads = read(fd, buff, buff_size)) > 0) {
            if (write(STDOUT_FILENO, buff, nreads) == -1) {
                fprintf(stderr, "Write error: %s\n", strerror(errno));
                free(buff);
                close(fd);
                return 1;
            }
        }
    } else if (argc == 4 && !strcmp(argv[2], "into")) {
        // 리다이렉션하여 다른 파일에 출력하는 경우
        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Failed to open file %s: %s\n", argv[1], strerror(errno));
            return 1;
        }

        int out_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            fprintf(stderr, "Failed to open file %s: %s\n", argv[3], strerror(errno));
            close(fd);
            return 1;
        }

        int saved_stdout = dup(STDOUT_FILENO);
        if (dup2(out_fd, STDOUT_FILENO) == -1) {
            fprintf(stderr, "Failed to duplicate file descriptor: %s\n", strerror(errno));
            close(fd);
            close(out_fd);
            return 1;
        }

        close(out_fd);

        if (fstat(fd, &file_stat) == -1) {
            fprintf(stderr, "Failed to get file size: %s\n", strerror(errno));
            close(fd);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
            return 1;
        }
        buff_size = file_stat.st_size > 0 ? file_stat.st_size : 16;
        buff = (char*)malloc(buff_size);
        if (!buff) {
            fprintf(stderr, "Failed to allocate memory\n");
            close(fd);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
            return 1;
        }

        while ((nreads = read(fd, buff, buff_size)) > 0) {
            if (write(STDOUT_FILENO, buff, nreads) == -1) {
                fprintf(stderr, "Write error: %s\n", strerror(errno));
                free(buff);
                close(fd);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
                return 1;
            }
        }

        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    } else {
        fprintf(stdout, "Wrong arguments\n");
        return 0;
    }

    // 메모리 해제 및 파일 닫기
    if (buff) free(buff);
    close(fd);

    return 0;
}
