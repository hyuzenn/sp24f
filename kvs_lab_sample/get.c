#include "kvs.h"

char* get(kvs_t* kvs, const char* key) {
    node_t* node = kvs->header;
    for (int i = kvs->kvs_mx_level; i >= 0; i--) {
        while (node->next[i] && strcmp(node->next[i]->key, key) < 0) {
            node = node->next[i];
        }
    }
    node = node->next[0];

    if (node && strcmp(node->key, key) == 0) {
        return node->value;
    }
    return NULL; //key가 발견되지 않았을 때.
}
