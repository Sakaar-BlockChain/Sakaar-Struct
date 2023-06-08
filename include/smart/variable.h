#ifndef VARIABLE_H
#define VARIABLE_H

#include "struct.h"

struct variable_st{
    struct string_st name;
    size_t position;
};

struct variable_st *variable_new(size_t);
void variable_set(struct variable_st *, const struct variable_st *);
void variable_clear(struct variable_st *);
void variable_free(struct variable_st *);

void variable_data_init(struct variable_st *);
void variable_data_free(struct variable_st *);

// TLV Methods
int variable_set_tlv(struct variable_st *, const struct string_st *);
void variable_get_tlv(const struct variable_st *, struct string_st *);

void print_variable(const struct variable_st *, int);


#endif //VARIABLE_H
