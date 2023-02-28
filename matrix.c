#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "container.h"
#include "matrix.h"
#include "vector.h"

matrix_t matrix_create(int rows, int cols){
    return (matrix_t) {
        container_create(rows*cols),
        false, rows, cols
    };
}

matrix_t matrix_from_container(rc_container_t* container, int rows, int cols){
    return (matrix_t) {
        container, 
        false, rows, cols,
    };
}

matrix_t matrix_from_2d_array(double** array, int rows, int cols){
    matrix_t mat = matrix_create(rows,cols);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            MATRIX_IDX(mat, i, j) = array[i][j];
        }
    }
    return mat;
}

void matrix_destroy(matrix_t* mat){
    container_remove_ref(mat->container);
    mat->container = NULL;
    mat->rows = -1;
    mat->cols = -1;
}

matrix_t matrix_identity(int n){
    matrix_t id = matrix_create(n, n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            MATRIX_IDX(id, i, j) = (i == j);
        }
    }
    return id;
}

matrix_t matrix_transpose(matrix_t mat){
    return (matrix_t) {
        container_view(mat.container),
        true, mat.cols, mat.rows 
    };
}

void matrix_summary(matrix_t mat){
    printf("cotainer: %p\nrows: %d, cols: %d\ntransposed: %s\n", (void*)mat.container, mat.rows, mat.cols, mat.transposed ? "yes" : "no");
}

void matrix_print(matrix_t mat){
    printf("[\n");
    for(int i = 0; i < mat.rows; i++){
        printf("\t[");
        for(int j = 0; j < mat.cols; j++){
            printf(" %.2lf ", MATRIX_IDX(mat,i,j));
        }
        printf("]\n");
    }
    printf("]\n");
}

matrix_t matrix_operation(matrix_t mat1, matrix_t mat2, container_operation op){
    assert(MATRIX_IS_VALID(mat1));
    assert(MATRIX_IS_VALID(mat2));
    assert(mat1.rows == mat2.rows);
    assert(mat1.cols == mat2.cols);

    rc_container_t* operated = (*op)(mat1.container, mat2.container, mat1.rows*mat1.cols);
    matrix_t mat = matrix_from_container(operated, mat1.rows, mat1.cols);
    return mat;
}

matrix_t matrix_sum(matrix_t mat1, matrix_t mat2){
    return matrix_operation(mat1, mat2, container_sum);
}

matrix_t matrix_subtract(matrix_t mat1, matrix_t mat2){
    return matrix_operation(mat1, mat2, container_subtract);
}
matrix_t matrix_multiply(matrix_t mat1, matrix_t mat2){
    return matrix_operation(mat1, mat2, container_multiply);
}
matrix_t matrix_divide(matrix_t mat1, matrix_t mat2){
    return matrix_operation(mat1, mat2, container_divide);
}

matrix_t matrix_matmult(matrix_t mat1, matrix_t mat2) {
    assert(mat1.cols == mat2.rows);
    int rows = mat1.rows;
    int cols = mat2.cols;

    matrix_t mat = matrix_create(rows, cols);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            double sum = 0;

            for(int k = 0; k < mat1.cols; k++){
                sum += MATRIX_IDX(mat1,i,k)*MATRIX_IDX(mat2,k,j);
            }
            printf("sum: %lf\n", sum);
            MATRIX_IDX(mat,i,j) = sum;
        }
    }
    return mat;
}

vector_t matrix_line(matrix_t mat, int i){
    return (vector_t) {
        container_view_at(mat.container, i*mat.cols),
        mat.cols
    };
}

void matrix_change_line(int i, matrix_t mat, vector_t line){
    assert(mat.cols == line.size);
    for(int j = 0; j < mat.cols; j++){
        MATRIX_IDX(mat, i, j) = VECTOR_IDX(line, j);
    }
}
