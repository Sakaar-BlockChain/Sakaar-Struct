#ifndef MEMORY_H
#define MEMORY_H

#include "stdlib.h"
#include <malloc.h>
#include <string.h>

struct object_st;
struct memory_st{
    size_t size;
    size_t filled;
    struct object_st *first_free;
    struct object_st *data;
};

struct object_st *memory_malloc();
void memory_free(struct object_st *);
void memory_clear();

#endif //MEMORY_H
