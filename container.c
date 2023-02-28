#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "container.h"

rc_container_t* container_create(int size){
    double* data = malloc(sizeof(double)*size);
    rc_container_t* container = malloc(sizeof(rc_container_t));
    assert(data != NULL);
    CONTAINER_DATA(container) = data;
    CONTAINER_OWNER(container) = container;
    CONTAINER_REFCOUNT(container) = 1;
    return container;
}

void container_destroy(rc_container_t* container){
    free(container);
}

void container_remove_ref(rc_container_t* container){
    if(CONTAINER_IS_VIEW(container)){
        container_remove_ref(CONTAINER_OWNER(container));
    }else{
        if(--CONTAINER_REFCOUNT(container) == 0) {
            container_destroy(container);
        }
    }
}

rc_container_t* container_view(rc_container_t* container){
    rc_container_t* memory_owner = CONTAINER_OWNER(container);
    CONTAINER_REFCOUNT(memory_owner)++;
    rc_container_t* view = malloc(sizeof(rc_container_t));

    CONTAINER_REFCOUNT(view) = -1;
    CONTAINER_OWNER(view) = memory_owner;
    CONTAINER_DATA(view) = CONTAINER_DATA(memory_owner);

    return view;
}

rc_container_t* container_view_at(rc_container_t* container, int start){
    rc_container_t* memory_owner = CONTAINER_OWNER(container);
    CONTAINER_REFCOUNT(memory_owner)++;
    rc_container_t* view = malloc(sizeof(rc_container_t));

    CONTAINER_REFCOUNT(view) = -1;
    CONTAINER_OWNER(view) = memory_owner;
    CONTAINER_DATA(view) = CONTAINER_DATA(memory_owner) + start;

    return view;
}

#define CONTAINER_GENERATE_VECTORIZED_OPERATION(name, op)\
rc_container_t* container_vectorized_ ##name  (rc_container_t* c1, rc_container_t* c2, int size){\
    rc_container_t* operated = container_create(size);\
    for(int i = 0; i < size; i++){\
        CONTAINER_DATA(operated)[i] = CONTAINER_DATA(c1)[i] op  CONTAINER_DATA(c2)[i];\
    }\
    return operated;\
}

CONTAINER_GENERATE_VECTORIZED_OPERATION(sum,+)
CONTAINER_GENERATE_VECTORIZED_OPERATION(subtract,-)
CONTAINER_GENERATE_VECTORIZED_OPERATION(multiply,*)
CONTAINER_GENERATE_VECTORIZED_OPERATION(divide,/)

#define CONTAINER_GENERATE_SCALAR_OPERATION(name, op)\
rc_container_t* container_scalar_ ##name  (rc_container_t* c1, double scalar, int size){\
    rc_container_t* operated = container_create(size);\
    for(int i = 0; i < size; i++){\
        CONTAINER_DATA(operated)[i] = CONTAINER_DATA(c1)[i] op scalar;\
    }\
    return operated;\
}

CONTAINER_GENERATE_SCALAR_OPERATION(sum,+)
CONTAINER_GENERATE_SCALAR_OPERATION(subtract,-)
CONTAINER_GENERATE_SCALAR_OPERATION(multiply,*)
CONTAINER_GENERATE_SCALAR_OPERATION(divide,/)

double container_reduce_sum(rc_container_t* c, int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += CONTAINER_DATA(c)[i];
    }
    return sum;
}
