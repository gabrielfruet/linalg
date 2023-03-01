#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <time.h>

#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a < b ? a : b

typedef void (*execution_function)();

void exec_time(execution_function fn);


#endif
