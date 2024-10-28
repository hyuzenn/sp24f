// kvs.h
#ifndef KVS_H
#define KVS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define MAX_LEVEL 10

// 노드 구조체 정의
struct node {
    char key[100];
    char* value;
    struct node* forward[MAX_LEVEL];
    int level;
};
typedef struct node node_t;

// Key-Value Store 구조체 정의
struct kvs {
    struct node* header;
    int level;
    int items;
};
typedef struct kvs kvs_t;

// 함수 선언
kvs_t* open_kvs();
int close_kvs(kvs_t* kvs);
int put(kvs_t* kvs, const char* key, const char* value);
char* get(kvs_t* kvs, const char* key);
int random_level(); // random_level 함수 선언 추가
node_t* create_node(int level, const char* key, const char* value); // create_node 함수 선언 추가

#endif // KVS_H
