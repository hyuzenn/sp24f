#include "kvs.h"

int close_kvs(kvs_t* kvs) {
    node_t* current = kvs->header->forward[0];
    while (current != NULL) {
        node_t* next = current->forward[0];
        free(current->value);
        free(current);
        current = next;
    }
    free(kvs->header);
    free(kvs);
    return 0;
}