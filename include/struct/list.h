#ifndef LIST_H
#define LIST_H

#include "struct.h"

// String Class
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
void list_resize(struct list_st *res, size_t size);
void list_append(struct list_st *res, struct object_st* obj);
void list_add_new(struct list_st *res, struct object_type *type);
void list_remove_last(struct list_st *res);

void list_sort(struct list_st *);

// TLV methods
void list_set_tlv(struct list_st *, const struct string_st *);
void list_get_tlv(const struct list_st *, struct string_st *);
void list_set_tlv_self(struct list_st *, const struct string_st *, struct object_type *);

void print_list(const struct list_st *, int tabs);

#endif //LIST_H
