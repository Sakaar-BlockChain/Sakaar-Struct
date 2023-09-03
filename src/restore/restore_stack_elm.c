#include "restore.h"

struct restore_stack_elm_st *restore_stack_elm_new() {
    struct restore_stack_elm_st *res = malloc(sizeof(struct restore_stack_elm_st));
    string_data_init(&res->hash);
    res->count = 0;
    res->next = NULL;
    return res;
}
void restore_stack_elm_free(struct restore_stack_elm_st *res) {
    if (res == NULL) return;
    string_data_free(&res->hash);
    free(res);
}

void restore_stack_elm_set(struct restore_stack_elm_st *res, const struct restore_stack_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_stack_elm_clear(res);
    string_set(&res->hash, &a->hash);
    res->count = a->count;
}
void restore_stack_elm_copy(struct restore_stack_elm_st *res, const struct restore_stack_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_stack_elm_clear(res);
    string_copy(&res->hash, &a->hash);
    res->count = a->count;
}

void restore_stack_elm_clear(struct restore_stack_elm_st *res) {
    if (res == NULL) return;
    string_clear(&res->hash);
    res->count = 0;
}
int8_t restore_stack_elm_cmp(const struct restore_stack_elm_st *obj1, const struct restore_stack_elm_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash) || obj1->count != obj2->count) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void restore_stack_elm_data_init(struct restore_stack_elm_st *res) {
    if (res == NULL) return;
    string_data_init(&res->hash);
    res->count = 0;
    res->next = NULL;
}
void restore_stack_elm_data_free(struct restore_stack_elm_st *res) {
    if (res == NULL) return;
    string_data_free(&res->hash);
}