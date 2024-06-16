#include "matrix.h"
#include <string.h>
#include <math.h>

Matrix_t* matrixNew(uint16_t rows, uint16_t cols)
{
    float* data = matrixAlloc(sizeof(float) * rows * cols);
    Matrix_t* matrix = matrixAlloc(sizeof(Matrix_t));
    if ((matrix != NULL) && (data != NULL))
    {
        matrix->rows = rows;
        matrix->cols = cols;
        matrix->data = data;
    }
    else
    {
        free(data);
        free(matrix);
        matrix = NULL;
    }

    return matrix;
}

void matrixMultiply(const Matrix_t *left, const Matrix_t* right, Matrix_t* result)
{
    const uint32_t resultNbOfElements = result->rows * result->cols;

    if ((left == result) || (right == result))
    {
        // Try to create an auxiliary matrix
        Matrix_t* aux = matrixNew(result->rows, result->cols);

        // If successful, then proceed
        if (aux != NULL)
        {
            // Fill aux matrix with zeros, i.e.,
            // aux = 0
            memset(aux->data, 0, sizeof(float) * resultNbOfElements);

            // Calculate
            // aux = left * right
            matrixMultiplyAddingToResult(left, right, aux);

            // Copy the result from aux, i.e.,
            // result = aux
            memcpy(result->data, aux->data, sizeof(float) * resultNbOfElements);

            // Free the allocated memory
            matrixFree(aux->data);
            matrixFree(aux);
        }
    }
    else
    {
        memset(result->data, 0, sizeof(float) * resultNbOfElements);
        matrixMultiplyAddingToResult(left, right, result);
    }
}

void matrixMultiplyAddingToResult(const Matrix_t *left, const Matrix_t* right, Matrix_t* result)
{
    uint16_t row, col, i;
    uint16_t resultIndex, leftIndex;

    for (row = 0; row < left->rows; row++)
    {
        leftIndex = left->cols * row;
        for (col = 0; col < right->cols; col++)
        {
            resultIndex = result->cols * row + col;
            for (i = 0; i < left->cols; i++)
            {
                result->data[resultIndex] += left->data[leftIndex + i] * right->data[right->cols * i + col];
            }
        }
    }
}

void matrixMinus(Matrix_t* m)
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

void matrixSum(const Matrix_t* left, const Matrix_t* right, Matrix_t* result)
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

void matrixSubtract(const Matrix_t* left, const Matrix_t* right, Matrix_t* result)
{
    uint16_t element;

    for (element = 0; element < (left->rows * left->cols); element++)
    {
        result->data[element] = left->data[element] - right->data[element];
    }
}

float matrixNorm2_2(const Matrix_t* matrix)
{
    float norm = 0;
    for (uint16_t element = 0; element < (matrix->rows * matrix->cols); element++)
        norm = norm + matrix->data[element] * matrix->data[element];

    return norm;
}

float matrixNorm2(const Matrix_t* matrix)
{
    return sqrt(matrixNorm2_2(matrix));
}
