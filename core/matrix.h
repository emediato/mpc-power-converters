#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Define here the matrix type. Common choices are:
 * - int32_t
 * - float
 * - double
 *
 * @warning Please do not use struct objects.
 */
typedef int32_t matrix_type;

/**
 * The matrix core object.
 */
typedef struct {
    uint16_t rows;
    uint16_t cols;
    matrix_type *data;

    /**
     * @brief Numeric base of the scalar.
     * It means what integer number represents the real number 1.0. It is particularly useful when working with int32_t.
     * For example, if 1024 was chosen as base, then 2048 means the real number 2.0.
     */
    matrix_type base;
} matrix_t;

// Dynamic allocation
#if defined(INC_FREERTOS_H)
#define matrixAlloc(bytes)  pvPortMalloc(bytes)
#define matrixFree(ptr)     vPortFree(ptr)
#else
#define matrixAlloc(bytes)  malloc(bytes)
#define matrixFree(ptr)     free(ptr)
#endif

#define matrixElement(m, r, c)  (m)->data[(m)->cols * r + c]

matrix_t* matrixNew(uint16_t rows, uint16_t cols, matrix_type base);

void matrixMultiply(const matrix_t *left, const matrix_t *right, matrix_t *result);

void matrixMultiplyAddingToResult(const matrix_t *left, const matrix_t *right, matrix_t *result);

void matrixMultiplyByScalar(const matrix_t* matrix, matrix_type scalar, matrix_t* result);

void matrixDivideByScalar(const matrix_t* matrix, matrix_type scalar, matrix_t* result);

/**
 * Implements result = left + right
 */
void matrixSum(const matrix_t *left, const matrix_t *right, matrix_t* result);

/**
 * Implements result = matrix + scalar
 * @param matrix is a valid pointer to a matrix_t object.
 * @param scalar is the scalar to be added to the matrix.
 * @param result is a valid pointer to a matrix_t object.
 */
void matrxSumScalar(const matrix_t* matrix, matrix_type scalar, matrix_t* result);

/**
 * Implements result = left - right
 */
void matrixSubtract(const matrix_t* left, const matrix_t* right, matrix_t* result);

/**
 * Calculates matrix * matrix.
 *
 * @warning It only supports vectors for the moment.
 */
matrix_type matrixNorm2_2(const matrix_t* matrix);

/**
 * Calculates sqrt(matrix * matrix).
 *
 * @warning It only supports vectors for the moment.
 */
matrix_type matrixNorm2(const matrix_t* matrix);

#ifdef __cplusplus
}
#endif

#endif //MATRIX_H
