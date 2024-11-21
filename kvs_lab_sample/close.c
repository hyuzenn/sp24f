#include "kvs.h"

int close(kvs_t* kvs)
{
	/* do program */
    if (!kvs) return -1;

    node_t* node = kvs->header->next[0];
    while (node) {
        node_t* next = node->next[0];
        free(node->value);
        free(node);
        node = next;
    }
    free(kvs->header);
    free(kvs);
    return 0;
}
