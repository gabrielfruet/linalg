#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdbool.h>
#include "vector.h"
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
matrix_t matrix_random_uniform_distribution(int rows, int cols, int seed);
matrix_t matrix_transpose(matrix_t mat);
void matrix_destroy(matrix_t* mat);
matrix_t matrix_identity(int n);
void matrix_summary(matrix_t mat);
void matrix_print(matrix_t mat);
matrix_t matrix_vec_sum(matrix_t mat1, matrix_t mat2);
matrix_t matrix_vec_subtract(matrix_t mat1, matrix_t mat2);
matrix_t matrix_vec_multiply(matrix_t mat1, matrix_t mat2);
matrix_t matrix_vec_divide(matrix_t mat1, matrix_t mat2);

void matrix_scalar_sum(matrix_t mat1, double scalar);
void matrix_scalar_subtract(matrix_t mat1, double scalar);
void matrix_scalar_multiply(matrix_t mat1, double scalar);
void matrix_scalar_divide(matrix_t mat1, double scalar);
matrix_t matrix_matmult(matrix_t mat1, matrix_t mat2);

void matrix_change_row(int i, matrix_t mat, vector_t row);
void matrix_change_col(int j, matrix_t mat, vector_t col);
void matrix_swap_row(int a, int b, matrix_t mat);
vector_t matrix_row(int i, matrix_t mat);
vector_t matrix_col(int j, matrix_t mat);

#define MATRIX_IDX(mat,i,j) CONTAINER_DATA(mat.container)[mat.transposed == true ? (j * mat.rows + i) : (i * mat.cols + j)]

#define MATRIX_IS_VALID(mat) (mat.container != NULL)

#define MATRIX_IS_SQUARE(mat) (mat.rows == mat.cols) && MATRIX_IS_VALID(mat)

#endif
