#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include "struct.h"

struct transaction_list_st{
    struct transaction_st **transactions;
    size_t size;
    size_t max_size;
};
// Standard operations
struct transaction_list_st *transaction_list_new();
void transaction_list_free(struct transaction_list_st *);

void transaction_list_set(struct transaction_list_st *, const struct transaction_list_st *);
void transaction_list_copy(struct transaction_list_st *, const struct transaction_list_st *);

void transaction_list_clear(struct transaction_list_st *);
int transaction_list_cmp(const struct transaction_list_st *, const struct transaction_list_st *);

// Data Methods
void transaction_list_data_init(struct transaction_list_st *);
void transaction_list_data_free(struct transaction_list_st *);

// Cmp Methods
int transaction_list_is_null(const struct transaction_list_st *);
void transaction_list_resize(struct transaction_list_st *, size_t);

// TLV Methods
int transaction_list_set_tlv(struct transaction_list_st *, const struct string_st *);
void transaction_list_get_tlv(const struct transaction_list_st *, struct string_st *);

// Attrib Methods
struct object_st *transaction_list_subscript(struct error_st *, struct transaction_list_st *, const struct object_st *);

#endif //TRANSACTION_LIST_H
