#include "operation.h"
//struct object_convert op_function_convert = {METHOD_CONVERT &op_function__bool, METHOD_CONVERT &op_function__int, METHOD_CONVERT &op_function__float};
//struct object_type op_function_type = {op_function_OP, NULL, NULL, &op_function_convert, &op_function_math};
struct object_tlv op_function_tlv = {METHOD_GET_TLV &op_function_get_tlv, METHOD_SET_TLV &op_function_set_tlv};
struct object_type op_function_type = {OP_FUNCTION_OP, &op_function_tlv};

struct op_function *op_function_new() {
    struct op_function *res = skr_malloc(OP_FUNCTION_SIZE);

    res->closure = NULL;
    res->argument = 0;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;

    return res;
}
void op_function_free(struct op_function *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);

    skr_free(res);
}

void op_function_set(struct op_function *res, const struct op_function *a) {
    if (res == NULL) return;
    op_function_clear(res);
    if (a == NULL) return;

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}
void op_function_copy(struct op_function *res, const struct op_function *a) {
    if (res == NULL) return;
    op_function_clear(res);
    if (a == NULL) return;

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}

void op_function_mark(struct op_function *res) {
    if (res == NULL) return;
    frame_mark(res->closure);
}
void op_function_unmark(struct op_function *res) {
    if (res == NULL) return;
    frame_unmark(res->closure);
}

void op_function_clear(struct op_function *res) {
    if (res == NULL) return;

    if(res->closure != NULL) frame_free(res->closure);
    res->closure = NULL;
    res->argument = 0;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;
}
int op_function_cmp(const struct op_function *obj1, const struct op_function *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->function_body != obj2->function_body ||
        obj1->func != obj2->func) return CMP_NEQ;
    return CMP_EQ;
}

void op_function_define(struct op_function *res, size_t function_body, struct parser_st *parser) {
    res->function_body = function_body;
    struct bytecode_st *code = parser->codes.bytecodes[function_body];
    res->argument = code->variable;
    res->argument_size = parser->variables.variable_lists[code->variable]->size - parser->closures.closures[code->closure]->data.size;

    struct frame_st *frame = res->closure = frame_new();
    struct variable_list_st data = parser->closures.closures[code->closure]->data;

    variable_list_set(&frame->attrib, &parser->closures.closures[code->closure]->attrib);
    list_resize(&frame->data, data.size);
    for (size_t i = 0, size = data.size; i < size; i++) {
        frame->data.data[i] = object_copy_obj(parser->var_stack->data[parser->var_start_pos + data.variables[i]->position]);
    }
}

// TLV Methods
int op_function_set_tlv(struct op_function *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    op_function_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_OP_FUNCTION) return ERR_TLV_TAG;

    res->closure = frame_new();
    struct integer_st position;
    integer_data_init(&position);

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = frame_set_tlv(res->closure, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->argument = integer_get_si(&position);

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->function_body = integer_get_si(&position);

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->argument_size = integer_get_si(&position);

    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
    return result;

}
void op_function_get_tlv(const struct op_function *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct integer_st position;
    integer_data_init(&position);
    struct string_st _tlv_data = {NULL, 0, 0};
    frame_get_tlv(res->closure, tlv);

    integer_set_ui(&position, res->argument);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->function_body);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->argument_size);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_OP_FUNCTION, tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
}
