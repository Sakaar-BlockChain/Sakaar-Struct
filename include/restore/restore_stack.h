#ifndef RESTORE_STACK_H
#define RESTORE_STACK_H

#include "struct.h"

struct restore_stack_st {
    size_t size;
    struct restore_stack_elm_st *front;
};

struct restore_stack_st *restore_stack_new();
void restore_stack_free(struct restore_stack_st *);

void restore_stack_clear(struct restore_stack_st *);

void restore_stack_pop_front(struct restore_stack_st *res);
void restore_stack_new_front(struct restore_stack_st *res);

#endif //RESTORE_STACK_H
