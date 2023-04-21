#include "basic.h"

struct object_sub block_sub = {NULL, METHOD_ATTRIB &block_attrib};
struct object_tlv block_tlv = {METHOD_GET_TLV &block_get_tlv, METHOD_SET_TLV &block_set_tlv};
struct object_type block_type = {BLOCK_OP, &block_tlv, &block_sub};

// Standard operations
struct block_st *block_new() {
    struct block_st *res = skr_malloc(sizeof(struct block_st));
    res->transactions = list_new();
    res->nodes_done = list_new();
    res->address_outside = string_new();
    res->hash = string_new();
    res->smart_contract = string_new();
    res->benefit = integer_new();
    res->time = integer_new();
    res->result_pros = integer_new();
    res->result_cons = integer_new();
    res->voted = integer_new();
    return res;
}
void block_free(struct block_st *res) {
    if (res == NULL) return;
    list_free(res->transactions);
    list_free(res->nodes_done);
    string_free(res->address_outside);
    string_free(res->hash);
    string_free(res->smart_contract);
    integer_free(res->benefit);
    integer_free(res->time);
    integer_free(res->result_pros);
    integer_free(res->result_cons);
    integer_free(res->voted);
    skr_free(res);
}

void block_set(struct block_st *res, const struct block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_clear(res);

    list_set(res->transactions, a->transactions);
    list_set(res->nodes_done, a->nodes_done);
    string_set(res->address_outside, a->address_outside);
    string_set(res->hash, a->hash);
    string_set(res->smart_contract, a->smart_contract);
    integer_set(res->benefit, a->benefit);
    integer_set(res->time, a->time);
    integer_set(res->result_pros, a->result_pros);
    integer_set(res->result_cons, a->result_cons);
    integer_set(res->voted, a->voted);
}
void block_copy(struct block_st *res, const struct block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_clear(res);

    list_copy(res->transactions, a->transactions);
    list_copy(res->nodes_done, a->nodes_done);
    string_copy(res->address_outside, a->address_outside);
    string_copy(res->hash, a->hash);
    string_copy(res->smart_contract, a->smart_contract);
    integer_copy(res->benefit, a->benefit);
    integer_copy(res->time, a->time);
    integer_copy(res->result_pros, a->result_pros);
    integer_copy(res->result_cons, a->result_cons);
    integer_copy(res->voted, a->voted);
}

void block_clear(struct block_st *res) {
    if (res == NULL) return;
    list_clear(res->transactions);
    list_clear(res->nodes_done);
    string_clear(res->address_outside);
    string_clear(res->hash);
    string_clear(res->smart_contract);
    integer_clear(res->benefit);
    integer_clear(res->time);
    integer_clear(res->result_pros);
    integer_clear(res->result_cons);
    integer_clear(res->voted);
}
int block_cmp(const struct block_st *obj1, const struct block_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(obj1->hash, obj2->hash) != 0 || integer_cmp(obj1->time, obj2->time) != 0) return 2;
    return 0;
}

// Cmp Methods
int block_is_null(const struct block_st *res) {
    return (res == NULL || string_is_null(res->hash));
}

// TLV Methods
void block_set_tlv(struct block_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    block_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_BLOCK) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();


    data = tlv_get_next_tlv(data, _tlv);
    list_set_tlv_self(res->transactions, _tlv, TRANSACTION_TYPE);

    data = tlv_get_next_tlv(data, _tlv);
    list_set_tlv_self(res->nodes_done, _tlv, STRING_TYPE);

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

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->result_pros, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->result_cons, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->voted, _tlv);

    string_free(_tlv);
}
void block_get_tlv(const struct block_st *block, struct string_st *res) {
    if (res == NULL) return;
    if (block == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    list_get_tlv(block->transactions, res);

    list_get_tlv(block->nodes_done, tlv);
    string_concat(res, tlv);

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

    integer_get_tlv(block->result_pros, tlv);
    string_concat(res, tlv);

    integer_get_tlv(block->result_cons, tlv);
    string_concat(res, tlv);

    integer_get_tlv(block->voted, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_BLOCK, res);
    string_free(tlv);
}

// Attrib Methods
struct object_st *block_attrib
(struct object_st *err, const struct block_st *block, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 12 && memcmp(str->data, "transactions", 12) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, block->transactions);
    }
    else if (str->size == 7 && memcmp(str->data, "benefit", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, block->benefit);
    }
    else if (str->size == 4 && memcmp(str->data, "time", 4) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, block->time);
    }
    else if (str->size == 15 && memcmp(str->data, "address_outside", 15) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, block->address_outside);
    }
    else if (str->size == 4 && memcmp(str->data, "hash", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, block->hash);
    }
    else if (str->size == 14 && memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, block->smart_contract);
    }
    else if (str->size == 10 && memcmp(str->data, "nodes_done", 10) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, block->nodes_done);
    }
    else if (str->size == 11 && memcmp(str->data, "result_pros", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, block->result_pros);
    }
    else if (str->size == 11 && memcmp(str->data, "result_cons", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, block->result_cons);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}