#ifndef LIST_H
#define LIST_H

#include "struct.h"

struct list_st{
    struct object_st **data;
    size_t size, max_size;
};
// Standard operations
struct list_st *list_new();
void list_free(struct list_st *);

void list_set(struct list_st *, const struct list_st *);
void list_copy(struct list_st *, const struct list_st *);

void list_mark(struct list_st *);
void list_unmark(struct list_st *);

void list_clear(struct list_st *);
int list_cmp(const struct list_st *, const struct list_st *);

// Cmp Methods
int list_is_null(const struct list_st *);

// Data Methods
void list_data_init(struct list_st *);
void list_data_free(struct list_st *);

// Class Methods
void list_resize(struct list_st *, size_t);
void list_append(struct list_st *, struct object_st* );
void list_concat(struct list_st *, const struct list_st *);
void list_add_new(struct list_st *, struct object_type *);
struct object_st *list_pop(struct list_st *);

void list_sort(struct list_st *);

// TLV Methods
int list_set_tlv(struct list_st *, const struct string_st *);
void list_get_tlv(const struct list_st *, struct string_st *);
int list_set_tlv_self(struct list_st *, const struct string_st *, struct object_type *);

void list__mul(struct object_st *, struct error_st *, const struct list_st *, const struct object_st *);
void list__add(struct object_st *, struct error_st *, const struct list_st *, struct object_st *);

// Convert Methods
void list__str(struct object_st *, struct error_st *, const struct list_st *);

// Convert Methods
struct object_st *list_subscript(struct error_st *, struct list_st *, const struct object_st *);

// Print
void print_list(const struct list_st *, int);


#endif //LIST_H
