#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
#include <omp.h>
#include <string.h>

#define M 1024
#define K 1024
#define N 1024
#define BLOCK_SIZE 32  // 캐시 효율성을 높이기 위한 블록 크기

float gen_random_float(float min, float max) {
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void transpose_matrix(float *B, float *B_T, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B_T[j * rows + i] = B[i * cols + j];
        }
    }
}

void matmul_optimized(float *A, float *B_T, float *C) {
    #pragma omp parallel for schedule(static) collapse(2)
    for (int i = 0; i < M; i += BLOCK_SIZE) {
        for (int j = 0; j < N; j += BLOCK_SIZE) {
            for (int bi = 0; bi < BLOCK_SIZE; bi++) {
                for (int bj = 0; bj < BLOCK_SIZE; bj++) {
                    float c = C[(i + bi) * N + (j + bj)];
                    for (int k = 0; k < K; k++) {
                        c += A[(i + bi) * K + k] * B_T[(j + bj) * K + k];
                    }
                    C[(i + bi) * N + (j + bj)] = c;
                }
            }
        }
    }
}

int main() {
    float *A, *B, *C, *B_T;

if (posix_memalign((void**)&A, 32, sizeof(float) * M * K) != 0) {
    fprintf(stderr, "Failed to allocate memory for A\n");
    return EXIT_FAILURE;
}
if (posix_memalign((void**)&B, 32, sizeof(float) * K * N) != 0) {
    fprintf(stderr, "Failed to allocate memory for B\n");
    return EXIT_FAILURE;
}
if (posix_memalign((void**)&C, 32, sizeof(float) * M * N) != 0) {
    fprintf(stderr, "Failed to allocate memory for C\n");
    return EXIT_FAILURE;
}
if (posix_memalign((void**)&B_T, 32, sizeof(float) * K * N) != 0) {
    fprintf(stderr, "Failed to allocate memory for B_T\n");
    return EXIT_FAILURE;
}

    srand((unsigned int)time(NULL));

    #pragma omp parallel for
    for (int i = 0; i < M * K; i++) A[i] = gen_random_float(0.0f, 0.1f);
    #pragma omp parallel for
    for (int i = 0; i < K * N; i++) B[i] = gen_random_float(0.0f, 0.1f);
    memset(C, 0, sizeof(float) * M * N);  // 메모리 초기화

    double start = omp_get_wtime();

    // B 전치
    transpose_matrix(B, B_T, K, N);

    // 행렬 곱
    matmul_optimized(A, B_T, C);

    double end = omp_get_wtime();
    printf("C[0] = %f\n", C[0]);
    printf("Execution Time: %f seconds\n", end - start);

    free(A);
    free(B);
    free(B_T);
    free(C);

    return 0;
}
