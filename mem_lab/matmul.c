#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h> // For SIMD
#include <omp.h>      // For OpenMP

#define M 1024
#define K 1024
#define N 1024

#define BM 64
#define BN 64
#define BK 64

float gen_random_float(float min, float max) {
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

int main() {
    float *A, *B, *C;

    // Memory allocation with alignment
    if (posix_memalign((void **)&A, 64, sizeof(float) * M * K) != 0 ||
        posix_memalign((void **)&B, 64, sizeof(float) * K * N) != 0 ||
        posix_memalign((void **)&C, 64, sizeof(float) * M * N) != 0) {
        printf("Failed to allocate memory\n");
        free(A);
        free(B);
        free(C);
        return 0;
    }

    srand((unsigned int)time(NULL));

    // Initialize A and B, zero-initialize C
    #pragma omp parallel for collapse(2)
    for (size_t rows = 0; rows < M; rows++) {
        for (size_t cols = 0; cols < K; cols++) {
            A[rows * K + cols] = gen_random_float(0.0f, 0.1f);
        }
    }

    #pragma omp parallel for collapse(2)
    for (size_t rows = 0; rows < K; rows++) {
        for (size_t cols = 0; cols < N; cols++) {
            B[rows * N + cols] = gen_random_float(0.0f, 0.1f);
        }
    }

    #pragma omp parallel for
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0.0f;
    }

    // Matrix multiplication with blocking and OpenMP
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < M; i += BM) {
        for (size_t j = 0; j < N; j += BN) {
            for (size_t k = 0; k < K; k += BK) {
                for (size_t ii = i; ii < i + BM && ii < M; ++ii) {
                    for (size_t jj = j; jj < j + BN && jj < N; ++jj) {
                        __m256 c_vec = _mm256_setzero_ps();
                        for (size_t kk = k; kk < k + BK && kk + 7 < K; kk += 8) {
                            __m256 a_vec = _mm256_load_ps(&A[ii * K + kk]);
                            __m256 b_vec = _mm256_load_ps(&B[kk * N + jj]);
                            c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                        }
                        // Horizontal addition to sum up the SIMD registers
                        float c_temp[8];
                        _mm256_store_ps(c_temp, c_vec);
                        float c_sum = c_temp[0] + c_temp[1] + c_temp[2] + c_temp[3] +
                                      c_temp[4] + c_temp[5] + c_temp[6] + c_temp[7];

                        for (size_t kk = k + (BK / 8) * 8; kk < k + BK && kk < K; ++kk) {
                            c_sum += A[ii * K + kk] * B[kk * N + jj];
                        }

                        #pragma omp atomic
                        C[ii * N + jj] += c_sum;
                    }
                }
            }
        }
    }

    // Print a single value for verification
    printf("%f\n", C[0]);

    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
