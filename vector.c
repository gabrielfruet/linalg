#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "container.h"

vector_t vector_create(int size){
    return (vector_t) {
        container_create(size),
        size
    };
}

vector_t vector_create_uninitialized(){
    return (vector_t) {
        NULL,
        -1
    };
}

/*
* The passed container will be borrowed.
*/
vector_t _vector_from_container(rc_container_t* c, int size){
    return (vector_t) {c,size};
}

vector_t vector_view(vector_t vec){
    return (vector_t) {
        container_view(CONTAINER_OWNER(vec.container)),
        vec.size
    };
}

vector_t vector_copy(vector_t vec){
    assert(VECTOR_IS_VALID(vec));
    vector_t vec_copy = vector_create(vec.size);
    for(int i = 0; i < vec.size; i++){
        VECTOR_IDX(vec_copy, i) = VECTOR_IDX(vec, i);
    }
    return vec_copy;
}

void vector_destroy(vector_t* vec){
    container_remove_ref(vec->container);
    vec->size = -1;
    vec->container = NULL;
}

void vector_print(vector_t vec){
    assert(VECTOR_IS_VALID(vec));
    printf("[");
    for(int i = 0; i < vec.size; i++){
        printf(" %.2lf ", VECTOR_IDX(vec, i));
    }
    printf("]\n");
}

vector_t vector_full(int size, double fill){
    vector_t vec = vector_create(size);
    for(int i = 0; i < size; i++){
        VECTOR_IDX(vec, i) = fill;
    }
    return vec;
}

vector_t vector_vec_operation(vector_t vec1, vector_t vec2, container_vec_operation op){
    assert(VECTOR_IS_VALID(vec1));
    assert(VECTOR_IS_VALID(vec2));
    assert(vec1.size == vec2.size || (vec1.size == 1 || vec2.size == 1));

    rc_container_t* operated = (*op)(vec1.container, vec2.container, vec1.size);

    vector_t vecop = _vector_from_container(operated, vec1.size);
    return vecop;
}

void vector_scalar_operation(vector_t vec1, double scalar, container_scalar_operation op){
    assert(VECTOR_IS_VALID(vec1));
    (*op)(vec1.container, scalar, vec1.size);
}

#define IMPL_SCALAR_OPERATION(op_name)\
void vector_scalar_ ## op_name(vector_t a, double b){ vector_scalar_operation(a, b, &container_scalar_ ## op_name ); }

#define IMPL_VEC_OPERATION(op_name)\
vector_t vector_vec_ ## op_name(vector_t a, vector_t b){ return vector_vec_operation(a, b, &container_vec_ ## op_name ); }

IMPL_VEC_OPERATION(sum)
IMPL_VEC_OPERATION(subtract)
IMPL_VEC_OPERATION(multiply)
IMPL_VEC_OPERATION(divide)

IMPL_SCALAR_OPERATION(sum)
IMPL_SCALAR_OPERATION(subtract)
IMPL_SCALAR_OPERATION(multiply)
IMPL_SCALAR_OPERATION(divide)


double vector_dot(vector_t vec1, vector_t vec2){
    vector_t multiplied = vector_vec_multiply(vec1, vec2);
    double result = container_reduce_sum(multiplied.container, multiplied.size);
    vector_destroy(&multiplied);
    return result;
}
