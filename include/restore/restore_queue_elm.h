#ifndef SAKAAR_RESTORE_QUEUE_ELM_H
#define SAKAAR_RESTORE_QUEUE_ELM_H

#include "struct.h"

struct restore_queue_elm_st {
    struct hash_time_st hash_time;
    struct generation_st *gen;
    int8_t typo;

    struct restore_queue_elm_st *next;
};

struct restore_queue_elm_st *restore_queue_elm_new();
void restore_queue_elm_free(struct restore_queue_elm_st *);

void restore_queue_elm_set(struct restore_queue_elm_st *, const struct restore_queue_elm_st *);
void restore_queue_elm_copy(struct restore_queue_elm_st *, const struct restore_queue_elm_st *);

void restore_queue_elm_clear(struct restore_queue_elm_st *);

// Data Methods
void restore_queue_elm_data_init(struct restore_queue_elm_st *);
void restore_queue_elm_data_free(struct restore_queue_elm_st *);

// TLV Methods
int8_t restore_queue_elm_set_tlv(struct restore_queue_elm_st *, const struct string_st *);
void restore_queue_elm_get_tlv(const struct restore_queue_elm_st *, struct string_st *);

#endif //SAKAAR_RESTORE_QUEUE_ELM_H
