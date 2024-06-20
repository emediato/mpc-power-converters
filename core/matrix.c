#include "matrix.h"
#include <string.h>
#include <math.h>

matrix_t* matrixNew(uint16_t rows, uint16_t cols, matrix_type base)
{
    matrix_type* data = matrixAlloc(sizeof(matrix_type) * rows * cols);
    matrix_t* matrix = matrixAlloc(sizeof(matrix_t));
    if ((matrix != NULL) && (data != NULL))
    {
        matrix->rows = rows;
        matrix->cols = cols;
        matrix->data = data;
        matrix->base = base;
    }
    else
    {
        free(data);
        free(matrix);
        matrix = NULL;
    }

    return matrix;
}

void matrixMultiply(const matrix_t *left, const matrix_t* right, matrix_t* result)
{
    const uint32_t resultNbOfElements = result->rows * result->cols;

    if ((left == result) || (right == result))
    {
        // Try to create an auxiliary matrix
        matrix_t* aux = matrixNew(result->rows, result->cols, left->base);

        // If successful, then proceed
        if (aux != NULL)
        {
            // Fill aux matrix with zeros, i.e.,
            // aux = 0
            memset(aux->data, 0, sizeof(matrix_type) * resultNbOfElements);

            // Calculate
            // aux = left * right
            matrixMultiplyAddingToResult(left, right, aux);

            // Copy the result from aux, i.e.,
            // result = aux
            memcpy(result->data, aux->data, sizeof(matrix_type) * resultNbOfElements);

            // Free the allocated memory
            matrixFree(aux->data);
            matrixFree(aux);
        }
    }
    else
    {
        memset(result->data, 0, sizeof(matrix_type) * resultNbOfElements);
        matrixMultiplyAddingToResult(left, right, result);
    }
}

void matrixMultiplyAddingToResult(const matrix_t *left, const matrix_t* right, matrix_t* result)
{
    uint16_t row, col, i;
    uint16_t resultIndex, leftIndex, rightIndex;

    for (row = 0; row < left->rows; row++)
    {
        leftIndex = left->cols * row;
        for (col = 0; col < right->cols; col++)
        {
            resultIndex = result->cols * row + col;
            for (i = 0; i < left->cols; i++)
            {
                rightIndex = right->cols * i + col;
                result->data[resultIndex] += (left->data[leftIndex + i] * right->data[rightIndex]) / left->base;
            }
        }
    }
}

void matrixMultiplyByScalar(const matrix_t* matrix, const matrix_type scalar, matrix_t* result)
{
    for (uint16_t element = 0; element < (matrix->rows * matrix->cols); element++)
        result->data[element] = matrix->data[element] * scalar;
}

void matrixDivideByScalar(const matrix_t* matrix, const matrix_type scalar, matrix_t* result)
{
    for (uint16_t element = 0; element < (matrix->rows * matrix->cols); element++)
        result->data[element] = matrix->data[element] / scalar;
}

void matrixMinus(matrix_t* m)
{
    uint16_t row, col;
    for (row = 0; row < m->rows; row++)
    {
        for (col = 0; col < m->cols; col++)
        {
            matrixElement(m, row, col) = -matrixElement(m, row, col);
        }
    }
}

void matrixSum(const matrix_t* left, const matrix_t* right, matrix_t* result)
{
    uint16_t element;

    if ((left->cols == right->cols) && (left->rows == right->rows))
    {
        for (element = 0; element < (left->rows * left->cols); element++)
        {
            result->data[element] = left->data[element] + right->data[element];
        }
    }
}

void matrxSumScalar(const matrix_t* matrix, const matrix_type scalar, matrix_t* result)
{
    for (uint16_t element = 0; element < (matrix->rows * matrix->cols); element++)
        result->data[element] = matrix->data[element] + scalar;
}

void matrixSubtract(const matrix_t* left, const matrix_t* right, matrix_t* result)
{
    uint16_t element;

    for (element = 0; element < (left->rows * left->cols); element++)
    {
        result->data[element] = left->data[element] - right->data[element];
    }
}

matrix_type matrixNorm2_2(const matrix_t* matrix)
{
    matrix_type norm = 0;
    for (uint16_t element = 0; element < (matrix->rows * matrix->cols); element++)
        norm = norm + (matrix->data[element] * matrix->data[element]) / matrix->base;

    return norm;
}

matrix_type matrixNorm2(const matrix_t* matrix)
{
    return sqrt(matrixNorm2_2(matrix));
}
