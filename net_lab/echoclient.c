#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char **argv) {
    int clientfd, port;
    struct sockaddr_in serveraddr;
    char buf[MAXLINE];
    ssize_t wbytes;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[2]);
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &serveraddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(1);
    }

    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        wbytes = write(clientfd, buf, strlen(buf));
        if (wbytes < (ssize_t)strlen(buf)) {
            fprintf(stderr, "Partial write error\n");
            break;
        }
    }

    close(clientfd);
    return 0;
}
