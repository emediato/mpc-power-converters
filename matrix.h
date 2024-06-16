#ifndef QUARK_UTILS_MATRIX_H
#define QUARK_UTILS_MATRIX_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Matrix {
    uint16_t rows;
    uint16_t cols;
    float *data;
} Matrix_t;

// Dynamic allocation
#if defined(INC_FREERTOS_H)
#define matrixAlloc(bytes)  pvPortMalloc(bytes)
#define matrixFree(ptr)     vPortFree(ptr)
#else
#define matrixAlloc(bytes)  malloc(bytes)
#define matrixFree(ptr)     free(ptr)
#endif

#define matrixElement(m, r, c)  (m)->data[(m)->cols * r + c]

Matrix_t* matrixNew(uint16_t rows, uint16_t cols);

void matrixMultiply(const Matrix_t *left, const Matrix_t *right, Matrix_t *result);

void matrixMultiplyAddingToResult(const Matrix_t *left, const Matrix_t *right, Matrix_t *result);

/**
 * Implements result = left + right
 */
void matrixSum(const Matrix_t *left, const Matrix_t *right, Matrix_t* result);

/**
 * Implements result = left - right
 */
void matrixSubtract(const Matrix_t* left, const Matrix_t* right, Matrix_t* result);

/**
 * Calculates matrix * matrix.
 *
 * @warning It only supports vectors for the moment.
 */
float matrixNorm2_2(const Matrix_t* matrix);

/**
 * Calculates sqrt(matrix * matrix).
 *
 * @warning It only supports vectors for the moment.
 */
float matrixNorm2(const Matrix_t* matrix);

#ifdef __cplusplus
}
#endif

#endif //QUARK_UTILS_MATRIX_H
