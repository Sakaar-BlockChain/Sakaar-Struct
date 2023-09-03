#ifndef RESTORE_STACK_ELM_H
#define RESTORE_STACK_ELM_H

#include "struct.h"

struct restore_stack_elm_st {
    struct string_st hash;
    size_t count;

    struct restore_stack_elm_st *next;
};

struct restore_stack_elm_st *restore_stack_elm_new();
void restore_stack_elm_free(struct restore_stack_elm_st *);

void restore_stack_elm_set(struct restore_stack_elm_st *, const struct restore_stack_elm_st *);
void restore_stack_elm_copy(struct restore_stack_elm_st *, const struct restore_stack_elm_st *);

void restore_stack_elm_clear(struct restore_stack_elm_st *);
int8_t restore_stack_elm_cmp(const struct restore_stack_elm_st *, const struct restore_stack_elm_st *);

// Data Methods
void restore_stack_elm_data_init(struct restore_stack_elm_st *);
void restore_stack_elm_data_free(struct restore_stack_elm_st *);

#endif //RESTORE_STACK_ELM_H
