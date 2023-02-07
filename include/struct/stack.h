#ifndef STACK_H
#define STACK_H

#include "struct.h"

// String Class
struct stack_el{
    struct object_st *data;
    struct stack_el *priv;
};
struct stack_el *stack_el_new();
void stack_el_free(struct stack_el *);


struct stack_st{
    struct stack_el *top;
    size_t size;
};
// Standard operations
struct stack_st *stack_new();
void stack_set(struct stack_st *, const struct stack_st *);
void stack_clear(struct stack_st *);
void stack_free(struct stack_st *);
int stack_cmp(const struct stack_st *, const struct stack_st *);

// Class methods
void stack_add(struct stack_st *res, struct object_st* obj);
void stack_add_new(struct stack_st *res, struct object_type *type);
void stack_pop(struct stack_st *res);

// Print
void print_stack(const struct stack_st *, int tabs);

#endif //STACK_H
