#include <stdlib.h>
#include <stdio.h>
#include "solve.h"
#include "vector.h"
#include "matrix.h"

void solve_upper_triangular(matrix_t mat){
    assert(MATRIX_IS_VALID(mat));

    for(int k = 0; k < mat.rows; k++){
        vector_t pivot_row = matrix_row(k, mat);
        double pivot = VECTOR_IDX(pivot_row, k);

        bool done = true;
        
        if(pivot == 0){
            done = false;
            for(int i = k+1; i < mat.cols && !done; i++){  
                if(MATRIX_IDX(mat, i, k) != 0){
                    matrix_swap_row(i, k, mat);
                    vector_destroy(&pivot_row);
                    pivot_row = matrix_row(k, mat);
                    pivot = VECTOR_IDX(pivot_row, k);
                    done = true;
                }
            }
        }

        if(done){
            for(int i = k+1; i < mat.rows; i++){
                vector_t row = matrix_row(i, mat);

                if(VECTOR_IDX(row, k) == 0) break;

                double multiplier = -pivot/VECTOR_IDX(row, k);

                vector_scalar_multiply(row, multiplier);
                vector_t zeroed_row = vector_vec_sum(pivot_row, row);
                vector_destroy(&row);
                matrix_change_row(i, mat, zeroed_row);
                vector_destroy(&zeroed_row);
            }
        }

        vector_destroy(&pivot_row);
    }
}
void solve_lower_triangular(matrix_t mat){
    assert(MATRIX_IS_VALID(mat));

    for(int k = mat.rows-1; k >= 0; k--){
        vector_t pivot_row = matrix_row(k, mat);
        double pivot = VECTOR_IDX(pivot_row, k);

        bool done = true;
        
        if(pivot == 0){
            done = false;
            for(int i = k-1; i >= 0 && !done; i--){  
                if(MATRIX_IDX(mat, i, k) != 0){
                    matrix_swap_row(i, k, mat);
                    vector_destroy(&pivot_row);
                    pivot_row = matrix_row(k, mat);
                    pivot = VECTOR_IDX(pivot_row, k);
                    done = true;
                }
            }
        }

        if(done){
            for(int i = k-1; i >= 0; i--){
                vector_t row = matrix_row(i, mat);

                if(VECTOR_IDX(row, i) == 0) break;

                double multiplier = -pivot/VECTOR_IDX(row, k);

                vector_scalar_multiply(row, multiplier);
                vector_t zeroed_row = vector_vec_sum(pivot_row, row);
                vector_destroy(&row);
                matrix_change_row(i, mat, zeroed_row);
                vector_destroy(&zeroed_row);
            }
        }

        vector_destroy(&pivot_row);
    }
}

void solve_diagonal(matrix_t mat){
    solve_upper_triangular(mat);
    solve_lower_triangular(mat);
}

void solve_linear_equation(matrix_t mat){
    assert(mat.rows + 1 == mat.cols);

    solve_diagonal(mat);

    for(int i = 0; i < mat.rows; i++){
        vector_t row = matrix_row(i, mat);
        double divisor = VECTOR_IDX(row, i);

        if(divisor == 0) continue;

        vector_scalar_divide(row, divisor);
        matrix_change_row(i, mat, row);
        vector_destroy(&row);
    }
}
