#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "container.h"

typedef struct rc_list{
    rc_container_t* container;
    struct rc_list* next;
}rc_list_t;

rc_list_t* rc_head = NULL;

void list_append(rc_container_t* container){
    rc_list_t* node = malloc(sizeof(rc_list_t));
    node->container = container;
    node->next = rc_head;
    rc_head = node;
}

void list_remove(rc_container_t* container){
    if(rc_head == NULL) return;

    rc_list_t* it = rc_head;

    if(it->container == container){
        rc_list_t* to_delete = rc_head;
        rc_head = rc_head->next;
        free(to_delete);
    }else{
        while(it->next && it->next->container != container){
            it = it->next;
        }
        if(it->next){
            rc_list_t* to_delete = it->next;
            it->next = it->next->next;
            free(to_delete);
        }
    }
}

int list_len(){
    rc_list_t* it = rc_head;
    int size = 0;
    while(it){
        it = it->next;
        size++;
    }
    return size;
}

rc_container_t* container_create(int size){
    double* data = malloc(sizeof(double)*size);
    rc_container_t* container = malloc(sizeof(rc_container_t));
    assert(data != NULL);
    CONTAINER_REFCOUNT(container) = 1;
    CONTAINER_DATA(container) = data;
    CONTAINER_OWNER(container) = container;
    list_append(container);
    return container;
}

void container_destroy(rc_container_t* container){
    list_remove(container);

    if(CONTAINER_IS_VIEW(container) == false)
        free(container->data);

    free(container);
}

void container_remove_ref(rc_container_t* container){
    if(CONTAINER_IS_VIEW(container)){
        container_remove_ref(CONTAINER_OWNER(container));
        container_destroy(container);
    }else{
        CONTAINER_REFCOUNT(container)--;
        if(CONTAINER_REFCOUNT(container) == 0) {
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
    list_append(view);

    return view;
}

rc_container_t* container_range(rc_container_t* container, int start, int end, int step){
    rc_container_t* range = container_create((end-start)/step);
    for(int i = start, j = 0; i < end; i += step, j++){
        CONTAINER_DATA(range)[j] = CONTAINER_DATA(container)[i];
    }
    return range;
}

rc_container_t* container_view_at(rc_container_t* container, int start){
    rc_container_t* memory_owner = CONTAINER_OWNER(container);
    CONTAINER_REFCOUNT(memory_owner)++;
    rc_container_t* view = malloc(sizeof(rc_container_t));

    CONTAINER_REFCOUNT(view) = -1;
    CONTAINER_OWNER(view) = memory_owner;
    CONTAINER_DATA(view) = CONTAINER_DATA(container) + start;

    list_append(view);

    return view;
}

#define IMPL_VECTORIZED_OPERATION(name, op)\
rc_container_t* container_vec_ ##name  (rc_container_t* c1, rc_container_t* c2, int size){\
    rc_container_t* operated = container_create(size);\
    for(int i = 0; i < size; i++){\
        CONTAINER_DATA(operated)[i] = CONTAINER_DATA(c1)[i] op  CONTAINER_DATA(c2)[i];\
    }\
    return operated;\
}

IMPL_VECTORIZED_OPERATION(sum,+)
IMPL_VECTORIZED_OPERATION(subtract,-)
IMPL_VECTORIZED_OPERATION(multiply,*)
IMPL_VECTORIZED_OPERATION(divide,/)

#define IMPL_SCALAR_OPERATION(name, op)\
void container_scalar_ ##name  (rc_container_t* c1, double scalar, int size){\
    for(int i = 0; i < size; i++){\
        CONTAINER_DATA(c1)[i] op ## = scalar;\
    }\
}

IMPL_SCALAR_OPERATION(sum,+)
IMPL_SCALAR_OPERATION(subtract,-)
IMPL_SCALAR_OPERATION(multiply,*)
IMPL_SCALAR_OPERATION(divide,/)

double container_reduce_sum(rc_container_t* c, int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += CONTAINER_DATA(c)[i];
    }
    return sum;
}
