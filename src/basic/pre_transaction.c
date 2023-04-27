#include "basic.h"

struct object_type pre_transaction_type = {PRE_TRANSACTION_OP};

// Standard operations
struct pre_transaction *pre_transaction_new() {
    struct pre_transaction *res = skr_malloc(sizeof(struct currency_st));
    string_data_init(&res->address_from);
    string_data_init(&res->address_to);
    string_data_init(&res->currency);

    integer_data_init(&res->balance);
    integer_data_init(&res->fee);

    string_data_init(&res->private_key);
    return res;
}
void pre_transaction_free(struct pre_transaction *res) {
    if (res == NULL) return;
    string_data_free(&res->address_from);
    string_data_free(&res->address_to);
    string_data_free(&res->currency);

    integer_data_free(&res->balance);
    integer_data_free(&res->fee);

    string_data_free(&res->private_key);
    skr_free(res);
}

void pre_transaction_set(struct pre_transaction *res, const struct pre_transaction *a) {
    if (res == NULL) return;
    if (a == NULL) return pre_transaction_clear(res);

    string_set(&res->address_from, &a->address_from);
    string_set(&res->address_to, &a->address_to);
    string_set(&res->currency, &a->currency);

    integer_set(&res->balance, &a->balance);
    integer_set(&res->fee, &a->fee);

    string_set(&res->private_key, &a->private_key);
}
void pre_transaction_copy(struct pre_transaction *res, const struct pre_transaction *a) {
    if (res == NULL) return;
    if (a == NULL) return pre_transaction_clear(res);

    string_copy(&res->address_from, &a->address_from);
    string_copy(&res->address_to, &a->address_to);
    string_copy(&res->currency, &a->currency);

    integer_copy(&res->balance, &a->balance);
    integer_copy(&res->fee, &a->fee);

    string_copy(&res->private_key, &a->private_key);
}

void pre_transaction_clear(struct pre_transaction *res) {
    if (res == NULL) return;
    string_clear(&res->address_from);
    string_clear(&res->address_to);
    string_clear(&res->currency);

    integer_clear(&res->balance);
    integer_clear(&res->fee);

    string_clear(&res->private_key);
}
int pre_transaction_cmp(const struct pre_transaction *obj1, const struct pre_transaction *obj2) {
    if (obj1 == NULL || obj2 == NULL) return 2;
    if (string_cmp(&obj1->address_from, &obj2->address_from) != 0) return 2;
    if (string_cmp(&obj1->address_to, &obj2->address_to) != 0) return 2;
    if (string_cmp(&obj1->currency, &obj2->currency) != 0) return 2;

    if (integer_cmp(&obj1->balance, &obj2->balance) != 0) return 2;
    if (integer_cmp(&obj1->fee, &obj2->fee) != 0) return 2;

    if (string_cmp(&obj1->private_key, &obj2->private_key) != 0) return 2;
    return 0;
}
