#include "basic.h"

struct object_sub transaction_sub = {NULL, METHOD_ATTRIB &transaction_attrib};
struct object_tlv transaction_tlv = {METHOD_GET_TLV &transaction_get_tlv, METHOD_SET_TLV &transaction_set_tlv};
struct object_type transaction_type = {TRANSACTION_OP, &transaction_tlv, &transaction_sub};

// Standard operations
struct transaction_st *transaction_new() {
    struct transaction_st *res = skr_malloc(sizeof(struct transaction_st));
    string_data_init(&res->address_from);
    string_data_init(&res->address_to);
    string_data_init(&res->currency);

    integer_data_init(&res->balance);
    integer_data_init(&res->fee);

    integer_data_init(&res->balance_from);
    string_data_init(&res->hash_from);

    string_data_init(&res->signature);
    return res;
}
void transaction_free(struct transaction_st *res) {
    if (res == NULL) return;
    string_data_free(&res->address_from);
    string_data_free(&res->address_to);
    string_data_free(&res->currency);

    integer_data_free(&res->balance);
    integer_data_free(&res->fee);

    integer_data_free(&res->balance_from);
    string_data_free(&res->hash_from);

    string_data_free(&res->signature);
    skr_free(res);
}

void transaction_set(struct transaction_st *res, const struct transaction_st *a) {
    if (res == NULL) return;
    if (a == NULL) return transaction_clear(res);

    string_set(&res->address_from, &a->address_from);
    string_set(&res->address_to, &a->address_to);
    string_set(&res->currency, &a->currency);

    integer_set(&res->balance, &a->balance);
    integer_set(&res->fee, &a->fee);

    integer_set(&res->balance_from, &a->balance_from);
    string_set(&res->hash_from, &a->hash_from);

    string_set(&res->signature, &a->signature);
}
void transaction_copy(struct transaction_st *res, const struct transaction_st *a) {
    if (res == NULL) return;
    if (a == NULL) return transaction_clear(res);

    string_copy(&res->address_from, &a->address_from);
    string_copy(&res->address_to, &a->address_to);
    string_copy(&res->currency, &a->currency);

    integer_copy(&res->balance, &a->balance);
    integer_copy(&res->fee, &a->fee);

    integer_copy(&res->balance_from, &a->balance_from);
    string_copy(&res->hash_from, &a->hash_from);

    string_copy(&res->signature, &a->signature);
}

void transaction_clear(struct transaction_st *res) {
    if (res == NULL) return;
    string_clear(&res->address_from);
    string_clear(&res->address_to);
    string_clear(&res->currency);

    integer_clear(&res->balance);
    integer_clear(&res->fee);

    integer_clear(&res->balance_from);
    string_clear(&res->hash_from);

    string_clear(&res->signature);
}
int transaction_cmp(const struct transaction_st *obj1, const struct transaction_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return 2;
    if (string_cmp(&obj1->address_from, &obj2->address_from) != 0) return 2;
    if (string_cmp(&obj1->address_to, &obj2->address_to) != 0) return 2;
    if (string_cmp(&obj1->currency, &obj2->currency) != 0) return 2;

    if (integer_cmp(&obj1->balance, &obj2->balance) != 0) return 2;
    if (integer_cmp(&obj1->fee, &obj2->fee) != 0) return 2;

    if (integer_cmp(&obj1->balance_from, &obj2->balance_from) != 0) return 2;
    if (string_cmp(&obj1->hash_from, &obj2->hash_from) != 0) return 2;

    if (string_cmp(&obj1->signature, &obj2->signature) != 0) return 2;
    return 0;
}

// TLV Methods
int transaction_set_tlv(struct transaction_st *res, const struct string_st *tlv) {
    if (res == NULL) return 0;
    transaction_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_TRANSACTION) return ERR_TLV_TAG;

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->address_from, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->address_to, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->currency, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = integer_set_tlv(&res->balance, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = integer_set_tlv(&res->fee, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = integer_set_tlv(&res->balance_from, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    {
        struct integer_st *num = integer_new();
        if ((result = integer_set_tlv(num, &_tlv_data)) != 0) goto end;
        integer_get_str(num, &res->hash_from);
        integer_free(num);
    }

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->signature, &_tlv_data)) != 0) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void transaction_get_tlv(const struct transaction_st *transaction, struct string_st *res) {
    if (res == NULL) return;
    if (transaction == NULL) return string_clear(res);

    struct string_st _tlv_data = {NULL, 0, 0};
    string_get_tlv(&transaction->address_from, res);

    string_get_tlv(&transaction->address_to, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&transaction->currency, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&transaction->balance, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&transaction->fee, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&transaction->balance_from, &_tlv_data);
    string_concat(res, &_tlv_data);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, &transaction->hash_from);
        integer_get_tlv(num, &_tlv_data);
        integer_free(num);
    }
    string_concat(res, &_tlv_data);

    string_get_tlv(&transaction->signature, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_TRANSACTION, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *transaction_attrib
(struct error_st *err, const struct transaction_st *transaction, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 12 && memcmp(str->data, "address_from", 12) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &transaction->address_from);
    }
    else if (str->size == 10 && memcmp(str->data, "address_to", 10) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &transaction->address_to);
    }
    else if (str->size == 8 && memcmp(str->data, "currency", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &transaction->currency);
    }
    else if (str->size == 7 && memcmp(str->data, "balance", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &transaction->balance);
    }
    else if (str->size == 3 && memcmp(str->data, "fee", 3) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &transaction->fee);
    }
    else if (str->size == 12 && memcmp(str->data, "balance_from", 12) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &transaction->balance_from);
    }
    else if (str->size == 9 && memcmp(str->data, "hash_from", 9) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &transaction->hash_from);
    }
    else if (str->size == 9 && memcmp(str->data, "signature", 9) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &transaction->signature);
    }
    else {
        object_free(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
