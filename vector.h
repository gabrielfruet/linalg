#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <assert.h>
#include "container.h"

typedef struct vector{
    rc_container_t* container; 
    int size;
}vector_t;

vector_t vector_create(int size);
vector_t vector_view(vector_t vec);
void vector_destroy(vector_t* vec);
vector_t vector_full(int size, double fill);
void vector_print(vector_t vec);

vector_t vector_vec_sum(vector_t vec1, vector_t vec2);
vector_t vector_vec_subtract(vector_t vec1, vector_t vec2);
vector_t vector_vec_multiply(vector_t vec1, vector_t vec2);
vector_t vector_vec_divide(vector_t vec1, vector_t vec2);

void vector_scalar_sum(vector_t vec1, double scalar);
void vector_scalar_subtract(vector_t vec1, double scalar);
void vector_scalar_multiply(vector_t vec1, double scalar);
void vector_scalar_divide(vector_t vec1, double scalar);

#define VECTOR_IS_VALID(vec) vec.container != NULL
#define VECTOR_IDX(vec, idx) CONTAINER_DATA(vec.container)[idx]

#endif
