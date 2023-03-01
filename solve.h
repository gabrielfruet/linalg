#ifndef __SOLVE_H__
#define __SOLVE_H__
#include "matrix.h"

void solve_upper_triangular(matrix_t mat);
void solve_lower_triangular(matrix_t mat);
void solve_diagonal(matrix_t mat);
void solve_linear_equation(matrix_t mat);

#endif
