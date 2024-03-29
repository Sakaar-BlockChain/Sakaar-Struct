#include "smart.h"

void token_list_set(struct token_list_st *res, const struct token_list_st *a) {
    if (res == NULL || res->type) return;
    token_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->tokens[i] = a->tokens[i];
}
void token_list_clear(struct token_list_st *res) {
    if (res == NULL) return;
    token_list_resize(res, 0);
}

void token_list_data_init(struct token_list_st *res) {
    if (res == NULL) return;
    res->tokens = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void token_list_data_free(struct token_list_st *res) {
    if (res == NULL) return;
    token_list_resize(res, 0);
    if (res->tokens != NULL) free(res->tokens);
}

void token_list_resize(struct token_list_st *res, size_t size) {
    if (res->tokens == NULL && size != 0) {
        res->max_size = size;
        res->tokens = malloc(sizeof(struct token_st *) * size);
        for (size_t i = 0; i < size; i++) res->tokens[i] = NULL;
    } else if (res->max_size < size) {
        res->tokens = realloc(res->tokens, sizeof(struct token_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->tokens[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
            if (res->tokens[i] != NULL) token_free(res->tokens[i]);
            res->tokens[i] = NULL;
        }
    }
    res->size = size;
}
void token_list_append(struct token_list_st *res, struct token_st *data) {
    if (res == NULL) return;
    token_list_resize(res, res->size + 1);
    res->tokens[res->size - 1] = data;
}
void token_list_add_new(struct token_list_st *res) {
    if (res == NULL || !res->type) return;
    token_list_resize(res, res->size + 1);
    res->tokens[res->size - 1] = token_new();
}
struct token_st *token_list_last(struct token_list_st *res) {
    if (res == NULL || res->tokens == NULL || res->size == 0) return NULL;
    return res->tokens[res->size - 1];
}