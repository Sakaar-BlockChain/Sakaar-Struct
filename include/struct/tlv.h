#ifndef TLV_H
#define TLV_H

#include "struct.h"
// Class Methods
void tlv_set_str(struct string_st *, unsigned, const char *, size_t);
void tlv_set_string(struct string_st *, unsigned, const struct string_st *);

size_t tlv_get_size_tag(const struct string_st *);
size_t tlv_get_size_head(const struct string_st *);
size_t tlv_get_size_value(const struct string_st *);

int32_t tlv_get_tag(const struct string_st *);
int8_t tlv_get_value(const struct string_st *, struct string_st *);

int8_t tlv_get_next_tlv(struct string_st *, struct string_st *);
void tlv_beautify(const struct string_st *, struct string_st *);
void tlv_from_beautify(const struct string_st *, struct string_st *);

// TLV Methods
int8_t tlv_set_tlv(struct string_st *, const struct string_st *);
void tlv_get_tlv(const struct string_st *, struct string_st *);

// Print
void print_tlv(const struct string_st *res);


#endif //TLV_H
