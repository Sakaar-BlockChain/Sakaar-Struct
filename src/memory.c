#include "memory.h"
#include "struct.h"


#define POOL_SIZE 65536
#define MEM_PTR(ptr) (*(void **) (ptr))

struct memory_st *pool = NULL;

void memory_init() {
    pool = malloc(sizeof(struct memory_st));
    pool->size = POOL_SIZE;
    pool->filled = 0;
    pool->first_free = NULL;
    pool->data = malloc(sizeof(struct object_st) * POOL_SIZE);
}


struct object_st *memory_malloc() {
    if (pool == NULL) memory_init();
    if (pool->size == pool->filled)
        return NULL;
    struct object_st *res = pool->first_free;

    if (res != NULL) pool->first_free = MEM_PTR(pool->first_free);
    else res = &pool->data[pool->filled++];

    memset(res, 0, sizeof (struct object_st));
    return res;
}

void memory_free(struct object_st *data) {
    if (pool->data <= data && data <= &pool->data[pool->filled]) {
        MEM_PTR(data) = pool->first_free;
        pool->first_free = data;
    }
}
void memory_clear() {
    if (pool == NULL) return;
    struct object_st *res = pool->first_free;
    while (res != NULL) {
        pool->first_free = MEM_PTR(pool->first_free);
        res->type = NONE_TYPE;
        res = pool->first_free;
    }

    for (size_t i = 0, size = pool->filled; i < size; i++) {
        res = &pool->data[i];
        if (res->type == NONE_TYPE) continue;
        object_clear(res);
    }

    free(pool->data);
    free(pool);
}