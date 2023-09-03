#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "struct.h"

struct linked_list_st{
    size_t size;

    struct linked_list_elm *front;
    struct linked_list_elm *back;
};

struct linked_list_st *linked_list_new();
void linked_list_free(struct linked_list_st *res);

void linked_list_set(struct linked_list_st *res, const struct linked_list_st *a);
void linked_list_copy(struct linked_list_st *res, const struct linked_list_st *a);

void linked_list_mark(struct linked_list_st *res);
void linked_list_unmark(struct linked_list_st *res);

void linked_list_clear(struct linked_list_st *res);
int8_t linked_list_cmp(const struct linked_list_st *obj1, const struct linked_list_st *obj2);

// Cmp Methods
int8_t linked_list_is_null(const struct linked_list_st *res);

// Data Methods
void linked_list_data_init(struct linked_list_st *res);
void linked_list_data_free(struct linked_list_st *res);

// Class Methods
void linked_list_push_front(struct linked_list_st *res, struct object_st *obj);
void linked_list_push_back(struct linked_list_st *res, struct object_st *obj);
void linked_list_pop_front(struct linked_list_st *res);
void linked_list_pop_back(struct linked_list_st *res);
void linked_list_new_front(struct linked_list_st *res, struct object_type *type);
void linked_list_new_back(struct linked_list_st *res, struct object_type *type);
void linked_list_concat(struct linked_list_st *res, const struct linked_list_st *a);
struct object_st *linked_list_front(struct linked_list_st *);
struct object_st *linked_list_back(struct linked_list_st *);

// TLV Methods
int8_t linked_list_set_tlv(struct linked_list_st *res, const struct string_st *tlv);
void linked_list_get_tlv(const struct linked_list_st *res, struct string_st *tlv);

// Convert Methods
struct object_st *linked_list_subscript(struct error_st *err, struct linked_list_st *linked_list, const struct object_st *obj);

// Convert Methods
void linked_list__str(struct object_st *res, struct error_st *err, const struct linked_list_st *obj);

// Math Methods
void linked_list__mul(struct object_st *res, struct error_st *err, const struct linked_list_st *obj1, const struct object_st *obj2);
void linked_list__add(struct object_st *res, struct error_st *err, const struct linked_list_st *obj1, struct object_st *obj2);

#endif //LINKED_LIST_H
