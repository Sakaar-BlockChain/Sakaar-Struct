#include "restore.h"

struct restore_block_st *restore_block_new() {
    struct restore_block_st *res = malloc(sizeof(struct restore_block_st));
    restore_list_data_init(&res->nodes);
    restore_list_resize(&res->nodes,
                        ((1 << (RESTORE_BLOCK_BIN * (RESTORE_BLOCK_HEIGHT))) - 1) / (RESTORE_BLOCK_CHILD - 1));

    res->block_id = 0;

    res->elm = res->nodes.data[res->nodes.size - 1];
    res->stack = NULL;
    return res;
}
void restore_block_free(struct restore_block_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);
    if (res->stack != NULL) restore_stack_free(res->stack);
    free(res);
}

void restore_block_set(struct restore_block_st *res, const struct restore_block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_block_clear(res);
    restore_list_set(&res->nodes, &a->nodes);

    res->elm = res->nodes.data[res->nodes.size - 1];
    res->block_id = a->block_id;
}
void restore_block_copy(struct restore_block_st *res, const struct restore_block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_block_clear(res);
    restore_list_copy(&res->nodes, &a->nodes);

    res->elm = res->nodes.data[res->nodes.size - 1];
    res->block_id = a->block_id;
}

void restore_block_clear(struct restore_block_st *res) {
    if (res == NULL) return;
    restore_list_clear(&res->nodes);
    restore_list_resize(&res->nodes,
                        ((1 << (RESTORE_BLOCK_BIN * (RESTORE_BLOCK_HEIGHT))) - 1) / (RESTORE_BLOCK_CHILD - 1));

    res->block_id = 0;

    res->elm = res->nodes.data[res->nodes.size - 1];
    if (res->stack != NULL) restore_stack_clear(res->stack);
}
int8_t restore_block_cmp(const struct restore_block_st *obj1, const struct restore_block_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->block_id != obj2->block_id) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void restore_block_data_init(struct restore_block_st *res) {
    if (res == NULL) return;
    restore_list_data_init(&res->nodes);
    restore_list_resize(&res->nodes,
                        ((1 << (RESTORE_BLOCK_BIN * (RESTORE_BLOCK_HEIGHT))) - 1) / (RESTORE_BLOCK_CHILD - 1));

    res->block_id = 0;

    res->elm = res->nodes.data[res->nodes.size - 1];
    res->stack = NULL;
}
void restore_block_data_free(struct restore_block_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);
    if (res->stack != NULL) restore_stack_free(res->stack);
}

// TLV Methods
int8_t restore_block_set_tlv(struct restore_block_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_block_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_RESTORE_BLOCK) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->block_id, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = restore_list_set_tlv(&res->nodes, &_tlv_data))) goto end;

    res->elm = res->nodes.data[res->nodes.size - 1];
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void restore_block_get_tlv(const struct restore_block_st *restore_block, struct string_st *res) {
    if (res == NULL) return;
    if (restore_block == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    size_get_tlv(restore_block->block_id, res);

    restore_list_get_tlv(&restore_block->nodes, &_tlv_data);
    string_concat(res, &_tlv_data);
    tlv_set_string(res, TLV_RESTORE_BLOCK, res);
    string_data_free(&_tlv_data);
}