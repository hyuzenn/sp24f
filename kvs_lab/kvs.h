#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdlib.h>
#include <limits.h>


#define MAX_LEVEL 10

typedef struct node {
    char *key;
    char *value;
    struct node **forward; // forward 포인터 배열
} node_t;

typedef struct skiplist {
    node_t *header;
    int level;
} skiplist_t;

typedef struct kvs{
	skiplist_t *list;
} kvs_t; 

node_t* createNode(int level, const char *key, const char *value);
skiplist_t* createSkiplist();
kvs_t* open_kvs(); 
int kvs_close(kvs_t* kvs); // free all memory space
int put(kvs_t* kvs, const char* key, const char* value); // return -1 if failed.
char* get(kvs_t* kvs, const char* key); // return NULL if not found. 