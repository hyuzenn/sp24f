#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_NUM 1000000

typedef double data_t;

struct vec {
    int len;
    data_t *data;
};
typedef struct vec vec_t;

int vec_length(vec_t* v) {
    assert(v);
    return v->len;
}

void combine(vec_t* v, data_t *dest) {
    assert(v);  // v가 NULL인지 확인
    assert(dest);
    *dest = 0;
    int len = vec_length(v);
    
    // Loop unrolling by 2
    for (int i = 0; i < len; i += 2) {
        if (i < len) {
            *dest += v->data[i]; // 첫 번째 요소
        }
        if (i + 1 < len) {
            *dest += v->data[i + 1]; // 두 번째 요소
        }
    }
}


void init(vec_t* v) {
    assert(v);
    v->len = MAX_NUM;
    v->data = (data_t*) malloc(sizeof(data_t) * MAX_NUM);
    
    // 메모리 할당 성공 여부 확인
    assert(v->data != NULL);

    // init
    for (int i = 0; i < MAX_NUM; i++)
        v->data[i] = (data_t)i;
}

int main() {
    printf("This is the naive version .. 1 \n");

    vec_t info;
    data_t result;

    // init 
    init(&info);

    // combine 
    combine(&info, &result);
    printf("Combined value = %f\n", result); // 결과 출력

    // 메모리 해제
    free(info.data);

    return 0;
}
