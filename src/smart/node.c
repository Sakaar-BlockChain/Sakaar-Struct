#include "smart.h"

struct node_st *node_new() {
    struct node_st *res = malloc(sizeof(struct node_st));
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_data_init(&res->tokens);
    node_list_data_init(&res->nodes);

    res->data = 0;
    res->variable = 0;
    res->closure = 0;
    return res;
}
void node_set(struct node_st *res, const struct node_st *a) {
    if (res == NULL) return;
    node_clear(res);

    res->type = a->type;
    res->sub_type = a->sub_type;

    token_list_set(&res->tokens, &a->tokens);
    node_list_set(&res->nodes, &a->nodes);

    res->data = a->data;
    res->variable = a->variable;
    res->closure = a->closure;
}
void node_clear(struct node_st *res) {
    if (res == NULL) return;
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_clear(&res->tokens);
    node_list_clear(&res->nodes);

    res->data = 0;
    res->variable = 0;
    res->closure = 0;
}
void node_free(struct node_st *res) {
    if (res == NULL) return;
    token_list_data_free(&res->tokens);
    node_list_data_free(&res->nodes);
    free(res);
}

void node_data_init(struct node_st *res) {
    if (res == NULL) return;
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_data_init(&res->tokens);
    node_list_data_init(&res->nodes);

    res->data = 0;
    res->variable = 0;
    res->closure = 0;
}
void node_data_free(struct node_st *res) {
    if (res == NULL) return;
    token_list_data_free(&res->tokens);
    node_list_data_free(&res->nodes);
}