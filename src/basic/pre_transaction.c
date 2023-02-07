#include "basic.h"

struct object_type pre_transaction_type = {PRE_TRANSACTION_OP};
// Standard operations
struct pre_transaction *pre_transaction_new() {
    struct pre_transaction *res = skr_malloc(PRE_TRANSACTION_SIZE);
    res->address_from = string_new();
    res->address_to = string_new();
    res->currency = string_new();

    res->balance = integer_new();
    res->fee = integer_new();

    res->private_key = string_new();
    return res;
}
void pre_transaction_set(struct pre_transaction *res, const struct pre_transaction *a) {
    if(a == NULL) return pre_transaction_clear(res);

    string_set(res->address_from, a->address_from);
    string_set(res->address_to, a->address_to);
    string_set(res->currency, a->currency);

    integer_set(res->balance, a->balance);
    integer_set(res->fee, a->fee);

    string_set(res->private_key, a->private_key);
}
void pre_transaction_clear(struct pre_transaction *res) {
    string_clear(res->address_from);
    string_clear(res->address_to);
    string_clear(res->currency);

    integer_clear(res->balance);
    integer_clear(res->fee);

    string_clear(res->private_key);
}
void pre_transaction_free(struct pre_transaction *res) {
    string_free(res->address_from);
    string_free(res->address_to);
    string_free(res->currency);

    integer_free(res->balance);
    integer_free(res->fee);

    string_free(res->private_key);
    skr_free(res);
}
