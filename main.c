#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "container.h"
#include "vector.h"
#include "matrix.h"
#include "solve.h"

void linear_solve(){
    matrix_t mat = matrix_random_uniform_distribution(1000,1001,3);
    matrix_scalar_multiply(mat, 100);

    solve_linear_equation(mat);
    matrix_destroy(&mat);
}

int main(){
    exec_time(&linear_solve);
    return 1;
}

