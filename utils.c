#include <time.h>
#include <stdio.h>
#include "utils.h"

void exec_time(execution_function fn){
    clock_t start = clock();
    (*fn)();
    clock_t end = clock();
    double ms = (double)(end-start)/(CLOCKS_PER_SEC) * 1000.0;
    printf("Elapsed time: %lf ms", ms);
}
