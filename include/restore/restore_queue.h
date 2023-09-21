#ifndef SAKAAR_RESTORE_QUEUE_H
#define SAKAAR_RESTORE_QUEUE_H

#include "struct.h"

struct restore_queue_st {
    struct restore_queue_elm_st *front;
    struct restore_queue_elm_st *back;
    size_t size;
};

struct restore_queue_st *restore_queue_new();
void restore_queue_free(struct restore_queue_st *res);

void restore_queue_set(struct restore_queue_st *res, const struct restore_queue_st *a);
void restore_queue_copy(struct restore_queue_st *res, const struct restore_queue_st *a);

void restore_queue_clear(struct restore_queue_st *res);
int8_t restore_queue_cmp(const struct restore_queue_st *obj1, const struct restore_queue_st *obj2);

// Data Methods
void restore_queue_data_init(struct restore_queue_st *res);
void restore_queue_data_free(struct restore_queue_st *res);

// Cmp Methods
int restore_queue_is_null(const struct restore_queue_st *res);
void restore_queue_push_back(struct restore_queue_st *);
void restore_queue_pop_front(struct restore_queue_st *);

// TLV Methods
int8_t restore_queue_set_tlv(struct restore_queue_st *, const struct string_st *);
void restore_queue_get_tlv(const struct restore_queue_st *, struct string_st *);

void print_restore_queue(const struct restore_queue_st *, int);



#endif //SAKAAR_RESTORE_QUEUE_H
