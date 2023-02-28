#include <stdio.h>
#include <stdlib.h>
#include "container.h"
#include "vector.h"
#include "matrix.h"

int main(){
    double** mat2darr1 = malloc(sizeof(double*)*2);
    double** mat2darr2 = malloc(sizeof(double*)*2);
    for(int i = 0; i < 2; i++){
        mat2darr1[i] = malloc(sizeof(double)*2);
        mat2darr2[i] = malloc(sizeof(double)*2);
    }

    mat2darr1[0][0] = 2;
    mat2darr1[0][1] = 3;
    mat2darr1[1][0] = 4;
    mat2darr1[1][1] = -1;


    mat2darr2[0][0] = 5;
    mat2darr2[0][1] = -1;
    mat2darr2[1][0] = 2;
    mat2darr2[1][1] = 3;

    matrix_t mat1 = matrix_from_2d_array(mat2darr1, 2, 2);
    matrix_t mat2 = matrix_from_2d_array(mat2darr2, 2, 2);

    matrix_print(mat1);
    matrix_print(mat2);

    matrix_t mat3 = matrix_matmult(mat1, mat2);

    matrix_print(mat3);

    return 1;
}
