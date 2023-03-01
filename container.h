#ifndef __CONTAINER_H__
#define __CONTAINER_H__
#include <stdbool.h>

typedef struct rc_container {
    struct rc_container* owner;
    int ref_count;
    double* data;
}rc_container_t;

typedef rc_container_t* (*container_vec_operation)(rc_container_t*, rc_container_t*, int);
typedef void (*container_scalar_operation)(rc_container_t*, double, int);

rc_container_t* container_create(int size);
void container_destroy(rc_container_t* container);
void container_remove_ref(rc_container_t* container);
rc_container_t* container_view(rc_container_t* container);
rc_container_t* container_range(rc_container_t* container, int start, int stop, int step);
rc_container_t* container_view_at(rc_container_t* container, int start);

rc_container_t* container_vec_sum(rc_container_t* c1, rc_container_t* c2, int size);
rc_container_t* container_vec_subtract(rc_container_t* c1, rc_container_t* c2, int size);
rc_container_t* container_vec_multiply(rc_container_t* c1, rc_container_t* c2, int size);
rc_container_t* container_vec_divide(rc_container_t* c1, rc_container_t* c2, int size);

void container_scalar_sum(rc_container_t* c1, double scalar, int size);
void container_scalar_subtract(rc_container_t* c1, double scalar, int size);
void container_scalar_multiply(rc_container_t* c1, double scalar, int size);
void container_scalar_divide(rc_container_t* c1, double scalar, int size);
double container_reduce_sum(rc_container_t* c, int size);

#define CONTAINER_REFCOUNT(container) (container->ref_count)
#define CONTAINER_IS_VIEW(container) (container->ref_count == -1)
#define CONTAINER_OWNER(container) container->owner
#define CONTAINER_DATA(container) container->data

#endif


