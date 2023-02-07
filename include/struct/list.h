#ifndef LIST_H
#define LIST_H

#include "struct.h"

struct list_st{
    struct object_st **data;
    size_t size, max_size;
};
// Standard operations
struct list_st *list_new();
void list_set(struct list_st *, const struct list_st *);
void list_clear(struct list_st *);
void list_free(struct list_st *);
int list_cmp(const struct list_st *, const struct list_st *);
int list_is_null(const struct list_st *);

// Class methods
void list_resize(struct list_st *, size_t);
void list_append(struct list_st *, struct object_st* );
void list_concat(struct list_st *, const struct list_st *);
void list_add_new(struct list_st *, struct object_type *);

void list_sort(struct list_st *);

// TLV methods
void list_set_tlv(struct list_st *, const struct string_st *);
void list_get_tlv(const struct list_st *, struct string_st *);
void list_set_tlv_self(struct list_st *, const struct string_st *, struct object_type *);

void list__mul(struct object_st *, const struct list_st *, const struct object_st *);
void list__add(struct object_st *, const struct list_st *, const struct object_st *);

// Convert methods
void list__str(struct object_st *, const struct list_st *);

// Print
void print_list(const struct list_st *, int);


#endif //LIST_H
