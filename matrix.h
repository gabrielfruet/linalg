#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdbool.h>
#include "container.h"

typedef struct matrix{
    rc_container_t* container;
    bool transposed;
    int rows;
    int cols;
}matrix_t;

matrix_t matrix_create(int rows, int cols);
matrix_t matrix_from_container(rc_container_t* container, int rows, int cols);
matrix_t matrix_from_2d_array(double** array, int rows, int cols);
matrix_t matrix_transpose(matrix_t mat);
void matrix_destroy(matrix_t* mat);
matrix_t matrix_identity(int n);
void matrix_summary(matrix_t mat);
void matrix_print(matrix_t mat);
matrix_t matrix_sum(matrix_t mat1, matrix_t mat2);
matrix_t matrix_subtract(matrix_t mat1, matrix_t mat2);
matrix_t matrix_multiply(matrix_t mat1, matrix_t mat2);
matrix_t matrix_divide(matrix_t mat1, matrix_t mat2);
matrix_t matrix_matmult(matrix_t mat1, matrix_t mat2);

#define MATRIX_IDX(mat,i,j) CONTAINER_DATA(mat.container)[mat.transposed == true ? (j * mat.rows + i) : (i * mat.cols + j)]
#define MATRIX_IS_VALID(mat) mat.container != NULL

#endif
