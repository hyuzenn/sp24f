#include "kvs.h"

int put(kvs_t* kvs, const char* key, const char* value) {
    node_t* prev_node[MAX_LEVEL]; //MAX_LEVEL = 16
    node_t* node = kvs->header;

	//printf("put: %s, %s\n", key, value);

    for (int i = kvs->kvs_mx_level; i >= 0; i--) { 
        while (node->next[i] && strcmp(node->next[i]->key, key) < 0) {//key에 해당하는 값을 찾았거나 없을 경우
            node = node->next[i];
        }
        prev_node[i] = node;
    }
    node = node->next[0];

    if (node && strcmp(node->key, key) == 0) { //이미 해당 노드가 있다면 값을 업데이트하기만 함.
        free(node->value);
        node->value = strdup(value);
        return 0;
    }

	else {
        int level = rand_lv();
        if (level > kvs->kvs_mx_level) { //최상위 레벨이 갱신되면서 현재 원소가 해당 레벨의 첫 원소가 됨 -> 따라서 header 다음 원소가 된다.
            for (int i = kvs->kvs_mx_level + 1; i <= level; i++) {
                prev_node[i] = kvs->header;
            }
            kvs->kvs_mx_level = level;
        }
        node = (node_t*)malloc(sizeof(node_t));
        strcpy(node->key, key);
        node->value = strdup(value);

		//inserting new node
        for (int i = 0; i <= level; i++) { //모든 레벨에 대해서 prev_node와 next사이에 new_node(node)를 삽입(prev_node -> new_node -> next)
            node->next[i] = prev_node[i]->next[i]; //new_node의 다음노드를 prev_node의 다음노드로 함.
            prev_node[i]->next[i] = node; //prev_node의 다음노드를 new_node로 함.
        }
        kvs->items++;
        return 0;
    }
}
