#include "basic.h"

struct object_sub block_sub = {NULL, METHOD_ATTRIB &block_attrib};
struct object_tlv block_tlv = {METHOD_GET_TLV &block_get_tlv, METHOD_SET_TLV &block_set_tlv};
struct object_type block_type = {BLOCK_OP, &block_tlv, &block_sub};

// Standard operations
struct block_st *block_new() {
    struct block_st *res = skr_malloc(sizeof(struct block_st));
    list_data_init(&res->transactions);
    list_data_init(&res->nodes_done);
    string_data_init(&res->address_outside);
    string_data_init(&res->hash);
    string_data_init(&res->smart_contract);
    integer_data_init(&res->benefit);
    integer_data_init(&res->time);
    integer_data_init(&res->result_pros);
    integer_data_init(&res->result_cons);
    integer_data_init(&res->voted);
    return res;
}
void block_free(struct block_st *res) {
    if (res == NULL) return;
    list_data_free(&res->transactions);
    list_data_free(&res->nodes_done);
    string_data_free(&res->address_outside);
    string_data_free(&res->hash);
    string_data_free(&res->smart_contract);
    integer_data_free(&res->benefit);
    integer_data_free(&res->time);
    integer_data_free(&res->result_pros);
    integer_data_free(&res->result_cons);
    integer_data_free(&res->voted);
    skr_free(res);
}

void block_set(struct block_st *res, const struct block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_clear(res);

    list_set(&res->transactions, &a->transactions);
    list_set(&res->nodes_done, &a->nodes_done);
    string_set(&res->address_outside, &a->address_outside);
    string_set(&res->hash, &a->hash);
    string_set(&res->smart_contract, &a->smart_contract);
    integer_set(&res->benefit, &a->benefit);
    integer_set(&res->time, &a->time);
    integer_set(&res->result_pros, &a->result_pros);
    integer_set(&res->result_cons, &a->result_cons);
    integer_set(&res->voted, &a->voted);
}
void block_copy(struct block_st *res, const struct block_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_clear(res);

    list_copy(&res->transactions, &a->transactions);
    list_copy(&res->nodes_done, &a->nodes_done);
    string_copy(&res->address_outside, &a->address_outside);
    string_copy(&res->hash, &a->hash);
    string_copy(&res->smart_contract, &a->smart_contract);
    integer_copy(&res->benefit, &a->benefit);
    integer_copy(&res->time, &a->time);
    integer_copy(&res->result_pros, &a->result_pros);
    integer_copy(&res->result_cons, &a->result_cons);
    integer_copy(&res->voted, &a->voted);
}

void block_mark(struct block_st *res) {
    if (res == NULL) return;
    list_mark(&res->transactions);
    list_mark(&res->nodes_done);
}
void block_unmark(struct block_st *res) {
    if (res == NULL) return;
    list_unmark(&res->transactions);
    list_unmark(&res->nodes_done);
}

void block_clear(struct block_st *res) {
    if (res == NULL) return;
    list_clear(&res->transactions);
    list_clear(&res->nodes_done);
    string_clear(&res->address_outside);
    string_clear(&res->hash);
    string_clear(&res->smart_contract);
    integer_clear(&res->benefit);
    integer_clear(&res->time);
    integer_clear(&res->result_pros);
    integer_clear(&res->result_cons);
    integer_clear(&res->voted);
}
int block_cmp(const struct block_st *obj1, const struct block_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash) || integer_cmp(&obj1->time, &obj2->time)) return CMP_NEQ;
    return CMP_EQ;
}

// Cmp Methods
int block_is_null(const struct block_st *res) {
    return (res == NULL || string_is_null(&res->hash));
}

// TLV Methods
int block_set_tlv(struct block_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    block_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_BLOCK) return ERR_TLV_TAG;

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;


    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = list_set_tlv_self(&res->transactions, &_tlv_data, TRANSACTION_TYPE))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = list_set_tlv_self(&res->nodes_done, &_tlv_data, STRING_TYPE))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address_outside, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    {
        struct integer_st *num = integer_new();
        if ((result = integer_set_tlv(num, &_tlv_data))) goto end;
        integer_get_str(num, &res->hash);
        integer_free(num);
    }

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->smart_contract, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->benefit, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->time, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->result_pros, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->result_cons, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->voted, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void block_get_tlv(const struct block_st *block, struct string_st *res) {
    if (res == NULL) return;
    if (block == NULL) return string_clear(res);

    struct string_st _tlv_data = {NULL, 0, 0};
    list_get_tlv(&block->transactions, res);

    list_get_tlv(&block->nodes_done, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&block->address_outside, &_tlv_data);
    string_concat(res, &_tlv_data);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, &block->hash);
        integer_get_tlv(num, &_tlv_data);
        integer_free(num);
    }
    string_concat(res, &_tlv_data);

    string_get_tlv(&block->smart_contract, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->benefit, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->time, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->result_pros, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->result_cons, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&block->voted, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_BLOCK, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *block_attrib
(struct error_st *err, const struct block_st *block, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 12 && memcmp(str->data, "transactions", 12) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, &block->transactions);
    }
    else if (str->size == 7 && memcmp(str->data, "benefit", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &block->benefit);
    }
    else if (str->size == 4 && memcmp(str->data, "time", 4) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &block->time);
    }
    else if (str->size == 15 && memcmp(str->data, "address_outside", 15) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &block->address_outside);
    }
    else if (str->size == 4 && memcmp(str->data, "hash", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &block->hash);
    }
    else if (str->size == 14 && memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &block->smart_contract);
    }
    else if (str->size == 10 && memcmp(str->data, "nodes_done", 10) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, &block->nodes_done);
    }
    else if (str->size == 11 && memcmp(str->data, "result_pros", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &block->result_pros);
    }
    else if (str->size == 11 && memcmp(str->data, "result_cons", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &block->result_cons);
    }
    else {
        object_free(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}