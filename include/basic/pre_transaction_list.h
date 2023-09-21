#ifndef PRE_TRANSACTION_LIST_H
#define PRE_TRANSACTION_LIST_H

#include "struct.h"

struct pre_transaction_list_st{
    struct pre_transaction **transactions;
    size_t size;
    size_t max_size;
};
// Standard operations
struct pre_transaction_list_st *pre_transaction_list_new();
void pre_transaction_list_free(struct pre_transaction_list_st *);

void pre_transaction_list_set(struct pre_transaction_list_st *, const struct pre_transaction_list_st *);
void pre_transaction_list_copy(struct pre_transaction_list_st *, const struct pre_transaction_list_st *);

void pre_transaction_list_clear(struct pre_transaction_list_st *);
int8_t pre_transaction_list_cmp(const struct pre_transaction_list_st *, const struct pre_transaction_list_st *);

// Data Methods
void pre_transaction_list_data_init(struct pre_transaction_list_st *);
void pre_transaction_list_data_free(struct pre_transaction_list_st *);

// Cmp Methods
int8_t pre_transaction_list_is_null(const struct pre_transaction_list_st *);
void pre_transaction_list_resize(struct pre_transaction_list_st *, size_t);

// Attrib Methods
struct object_st *pre_transaction_list_subscript(struct error_st *, struct pre_transaction_list_st *, const struct object_st *);

#endif //PRE_TRANSACTION_LIST_H
