#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

//#define MAX_NUM 10
#define MAX_NUM 100000000
#define UNROLL_FACTOR 4

typedef double data_t;

struct vec {
    int len;
    data_t *data;
};
typedef struct vec vec_t;

int get_vec_element(vec_t* v, int idx, data_t *val)
{
    assert(v);

    if (idx >= v->len)
        return 0;

    *val = v->data[idx];
    return 1;
}

int vec_length(vec_t* v)
{
    assert(v);
    return v->len;
}

void combine(vec_t* v, data_t *dest) {
    assert(dest);
    *dest = 0;
    int len = vec_length(v); 
    for (int i = 0; i < len; i += UNROLL_FACTOR) {
        *dest += v->data[i];
        if (i + 1 < len) *dest += v->data[i + 1];
        if (i + 2 < len) *dest += v->data[i + 2];
        if (i + 3 < len) *dest += v->data[i + 3];
    }
}

void init(vec_t* v)
{
    assert(v);
    v->len = MAX_NUM;
    v->data = (data_t*) malloc(sizeof(data_t) * MAX_NUM);
    
    // Check if malloc succeeded
    if (v->data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed!\n");
        exit(EXIT_FAILURE);  // Exit if malloc fails
    }

    // init 
    for (int i = 0; i < MAX_NUM; i++)
        v->data[i] = i;
}

void cleanup(vec_t* v) {
    assert(v);
    free(v->data);
    v->data = NULL; // Prevent dangling pointer
}

int main()
{
    printf("This is the naive version .. \n");
    printf("%d \n", MAX_NUM);

    vec_t info; 
    data_t result; 

    // init 
    init(&info);

    // combine 
    combine(&info, &result);
    printf("combined val = %f\n", result);

    // cleanup 
    cleanup(&info);

    return 0;
}