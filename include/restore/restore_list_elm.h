#ifndef RESTORE_LIST_ELM_H
#define RESTORE_LIST_ELM_H

#include "struct.h"
#include "basic.h"

struct restore_list_elm_st {
    struct hash_time_st hash_time;
    struct string_st hash;
};

struct restore_list_elm_st *restore_list_elm_new();
void restore_list_elm_free(struct restore_list_elm_st *);

void restore_list_elm_set(struct restore_list_elm_st *, const struct restore_list_elm_st *);
void restore_list_elm_copy(struct restore_list_elm_st *, const struct restore_list_elm_st *);

void restore_list_elm_clear(struct restore_list_elm_st *);
int8_t restore_list_elm_cmp(const struct restore_list_elm_st *, const struct restore_list_elm_st *);

// Data Methods
void restore_list_elm_data_init(struct restore_list_elm_st *);
void restore_list_elm_data_free(struct restore_list_elm_st *);

// TLV Methods
int8_t restore_list_elm_set_tlv(struct restore_list_elm_st *, const struct string_st *);
void restore_list_elm_get_tlv(const struct restore_list_elm_st *, struct string_st *);

#endif //RESTORE_LIST_ELM_H
