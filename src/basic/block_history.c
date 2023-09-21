#include "basic.h"

struct object_sub block_history_sub = {NULL, METHOD_ATTRIB &block_history_attrib};
struct object_tlv block_history_tlv = {METHOD_GET_TLV &block_history_get_tlv, METHOD_SET_TLV &block_history_set_tlv};
struct object_type block_history_type = {BLOCK_HISTORY_OP, &block_history_tlv, &block_history_sub};

// Standard operations
struct block_history *block_history_new() {
    struct block_history *res = malloc(sizeof(struct block_history));
    hash_time_data_init(&res->hash_time);
    transaction_list_data_init(&res->transactions);
    string_data_init(&res->address_outside);
    string_data_init(&res->smart_contract);
    integer_data_init(&res->benefit);
    res->result = 0;
    return res;
}
void block_history_free(struct block_history *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    transaction_list_data_free(&res->transactions);
    string_data_free(&res->address_outside);
    string_data_free(&res->smart_contract);
    integer_data_free(&res->benefit);
    free(res);
}

void block_history_set(struct block_history *res, const struct block_history *a) {
    if (a == NULL) return block_history_clear(res);

    hash_time_set(&res->hash_time, &a->hash_time);
    transaction_list_set(&res->transactions, &a->transactions);
    string_set(&res->address_outside, &a->address_outside);
    string_set(&res->smart_contract, &a->smart_contract);
    integer_set(&res->benefit, &a->benefit);
    res->result = a->result;
}
void block_history_copy(struct block_history *res, const struct block_history *a) {
    if (res == NULL) return;
    if (a == NULL) return block_history_clear(res);

    hash_time_copy(&res->hash_time, &a->hash_time);
    transaction_list_copy(&res->transactions, &a->transactions);
    string_copy(&res->address_outside, &a->address_outside);
    string_copy(&res->smart_contract, &a->smart_contract);
    integer_copy(&res->benefit, &a->benefit);
    res->result = a->result;
}

void block_history_clear(struct block_history *res) {
    if (res == NULL) return;
    hash_time_clear(&res->hash_time);
    transaction_list_clear(&res->transactions);
    string_clear(&res->address_outside);
    string_clear(&res->smart_contract);
    integer_clear(&res->benefit);
    res->result = 0;
}
int8_t block_history_cmp(const struct block_history *obj1, const struct block_history *obj2) {
    if (obj1 == NULL || obj2 == NULL || hash_time_cmp(&obj1->hash_time, &obj2->hash_time)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void block_history_data_init(struct block_history *res) {
    if (res == NULL) return;
    hash_time_data_init(&res->hash_time);
    transaction_list_data_init(&res->transactions);
    string_data_init(&res->address_outside);
    string_data_init(&res->smart_contract);
    integer_data_init(&res->benefit);
    res->result = 0;
}
void block_history_data_free(struct block_history *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    transaction_list_data_free(&res->transactions);
    string_data_free(&res->address_outside);
    string_data_free(&res->smart_contract);
    integer_data_free(&res->benefit);
}

// TLV Methods
int8_t block_history_set_tlv(struct block_history *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    block_history_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_BLOCK_HISTORY) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    size_t sub;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = hash_time_set_tlv(&res->hash_time, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = transaction_list_set_tlv(&res->transactions, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address_outside, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->smart_contract, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->benefit, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&sub, &_tlv_data))) goto end;
    res->result = (int8_t) sub;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void block_history_get_tlv(const struct block_history *block, struct string_st *res) {
    if (res == NULL) return;
    if (block == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    hash_time_get_tlv(&block->hash_time, res);

    transaction_list_get_tlv(&block->transactions, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&block->address_outside, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&block->smart_contract, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->benefit, &_tlv_data);
    string_concat(res, &_tlv_data);

    size_get_tlv(block->result, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_BLOCK_HISTORY, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *block_history_attrib
(struct error_st *err, const struct block_history *block, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    if (str->size == 12 && memcmp(str->data, "hash_time", 9) == 0) {
        object_set_type(res, HASH_TIME_TYPE);
        hash_time_set(res->data, &block->hash_time);
    }
    else if (str->size == 12 & memcmp(str->data, "transactions", 12) == 0) {
        object_set_type(res, TRANS_LIST_TYPE);
        transaction_list_set(res->data, &block->transactions);
    }
    else if (str->size == 7 & memcmp(str->data, "benefit", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &block->benefit);
    }
    else if (str->size == 15 & memcmp(str->data, "address_outside", 15) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &block->address_outside);
    }
    else if (str->size == 14 & memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &block->smart_contract);
    }
    else if (str->size == 6 & memcmp(str->data, "result", 6) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, block->result);
    }
    else {
        object_free(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
