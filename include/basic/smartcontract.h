#ifndef SMARTCONTRACT_H
#define SMARTCONTRACT_H

#include "struct.h"

// TLV Methods
int8_t smartcontract_set_tlv(struct string_st *, const struct string_st *);
void smartcontract_get_tlv(const struct string_st *, struct string_st *);

// Print
void print_smartcontract(const struct string_st *res);

#endif //SMARTCONTRACT_H
