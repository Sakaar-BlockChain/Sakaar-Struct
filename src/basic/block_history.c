#include "basic.h"

struct object_tlv block_history_tlv = {METHOD_GET_TLV &block_history_get_tlv, METHOD_SET_TLV &block_history_set_tlv};
struct object_type block_history_type = {BLOCK_HISTORY_OP, &block_history_tlv};
// Standard operations
struct block_history *block_history_new() {
    struct block_history *res = skr_malloc(BLOCK_HISTORY_SIZE);
    res->transactions = list_new();
    res->address_outside = string_new();
    res->hash = string_new();
    res->smart_contract = string_new();
    res->benefit = integer_new();
    res->time = integer_new();
    res->result = integer_new();
    return res;
}
void block_history_set(struct block_history *res, const struct block_history *a) {
    if (a == NULL) return block_history_clear(res);

    list_set(res->transactions, a->transactions);
    string_set(res->address_outside, a->address_outside);
    string_set(res->hash, a->hash);
    string_set(res->smart_contract, a->smart_contract);
    integer_set(res->benefit, a->benefit);
    integer_set(res->time, a->time);
    integer_set(res->result, a->result);
}
void block_history_clear(struct block_history *res) {
    list_clear(res->transactions);
    string_clear(res->address_outside);
    string_clear(res->hash);
    string_clear(res->smart_contract);
    integer_clear(res->benefit);
    integer_clear(res->time);
    integer_clear(res->result);
}
void block_history_free(struct block_history *res) {
    list_free(res->transactions);
    string_free(res->address_outside);
    string_free(res->hash);
    string_free(res->smart_contract);
    integer_free(res->benefit);
    integer_free(res->time);
    integer_free(res->result);
    skr_free(res);
}

// TLV Methods
void block_history_set_tlv(struct block_history *res, const struct string_st *tlv) {
    if (res == NULL) return;
    block_history_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_BLOCK_HISTORY) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    list_set_tlv_self(res->transactions, _tlv, TRANSACTION_TYPE);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->address_outside, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, res->hash);
        integer_free(num);
    }

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->smart_contract, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->benefit, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->time, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->result, _tlv);

    string_free(_tlv);
}
void block_history_get_tlv(const struct block_history *block, struct string_st *res) {
    if (res == NULL) return;
    if (block == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    list_get_tlv(block->transactions, res);

    string_get_tlv(block->address_outside, tlv);
    string_concat(res, tlv);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, block->hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    string_get_tlv(block->smart_contract, tlv);
    string_concat(res, tlv);

    integer_get_tlv(block->benefit, tlv);
    string_concat(res, tlv);

    integer_get_tlv(block->time, tlv);
    string_concat(res, tlv);

    integer_get_tlv(block->result, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_BLOCK_HISTORY, res);
    string_free(tlv);
}

