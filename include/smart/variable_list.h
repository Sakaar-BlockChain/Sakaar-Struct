#ifndef VARIABLE_LIST_H
#define VARIABLE_LIST_H

#include "struct.h"

struct variable_list_st{
    struct variable_st **variables;
    size_t size;
    size_t max_size;
    int8_t type;
};

struct variable_list_st *variable_list_new();
void variable_list_set(struct variable_list_st *, const struct variable_list_st *);
void variable_list_clear(struct variable_list_st *);
void variable_list_free(struct variable_list_st *);

void variable_list_data_init(struct variable_list_st *);
void variable_list_data_free(struct variable_list_st *);

void variable_list_resize(struct variable_list_st *, size_t);
void variable_list_append(struct variable_list_st *, struct variable_st *);
size_t variable_list_add_new(struct variable_list_st *);
struct variable_st *variable_list_last(struct variable_list_st *);

// TLV Methods
int8_t variable_list_set_tlv(struct variable_list_st *, const struct string_st *);
void variable_list_get_tlv(const struct variable_list_st *, struct string_st *);

void print_variable_list(const struct variable_list_st *, int);

#endif //VARIABLE_LIST_H
