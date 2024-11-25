#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MAX_LEVEL 8
#define MAX_KEY_LENGTH 1000
#define MAX_VALUE_LENGTH 10000

struct node {
	char key[MAX_KEY_LENGTH];
	char* value;
	struct node* next[MAX_LEVEL];
};	
typedef struct node node_t;

typedef struct {
    char key[MAX_KEY_LENGTH];  // 키
    char value[MAX_VALUE_LENGTH];  // 값
} KeyValuePair;

struct kvs{
	struct node* header; // database
	int items; // number of data 
	int kvs_mx_level;
	
	KeyValuePair* data;  // 데이터 배열
  int num_items;       // 저장된 항목 수
};
typedef struct kvs kvs_t; 

typedef struct kvs_iterator_t {
    node_t* current; // 현재 순회 중인 노드
		int level;        // 현재 레벨
} kvs_iterator_t;


kvs_t* kvs_open();
int kvs_close(kvs_t* kvs); // free all mem alloc
int put(kvs_t* kvs, const char* key, const char* value); 
char* get(kvs_t* kvs, const char* key); 
int rand_lv();

int do_snapshot(kvs_t* kvs);
int do_recovery(kvs_t* kvs);

kvs_iterator_t* kvs_iterator(kvs_t* kvs);
int kvs_has_next(kvs_iterator_t* it);
char* kvs_next_key(kvs_iterator_t* it);
void kvs_iterator_close(kvs_iterator_t* it);
node_t* search(node_t* head, const char* key);  