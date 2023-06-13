#include "operation.h"

struct object_sub op_class_sub = {METHOD_SUBSCRIPT &op_class_subscript, METHOD_ATTRIB &op_class_attrib};
//struct object_convert op_class_convert = {METHOD_CONVERT &op_class__bool, METHOD_CONVERT &op_class__int, METHOD_CONVERT &op_class__float, METHOD_CONVERT &op_class__str};
//struct object_type op_class_type = {op_class_OP, NULL, &op_class_sub, &op_class_convert};
struct object_tlv op_class_tlv = {METHOD_GET_TLV &op_class_get_tlv, METHOD_SET_TLV &op_class_set_tlv};
struct object_type op_class_type = {OP_CLASS_OP, &op_class_tlv, &op_class_sub};

struct op_class *op_class_new() {
    struct op_class *res = skr_malloc(OP_CLASS_SIZE);
    res->attr = NULL;

    res->closure = NULL;
    res->argument = 0;
    res->class_body = 0;

    return res;
}
void op_class_free(struct op_class *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);
    if(res->attr != NULL) frame_free(res->attr);

    skr_free(res);
}

void op_class_set(struct op_class *res, const struct op_class *a) {
    if (res == NULL) return;
    op_class_clear(res);
    if (a == NULL) return;
    if (a->attr != NULL) {
        res->attr = frame_new();
        frame_set(res->attr, a->attr);
    }

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}
void op_class_copy(struct op_class *res, const struct op_class *a) {
    if (res == NULL) return;
    op_class_clear(res);
    if (a == NULL) return;
    if (a->attr != NULL) {
        res->attr = frame_new();
        frame_set(res->attr, a->attr);
    }

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}

void op_class_mark(struct op_class *res) {
    if (res == NULL) return;
    frame_mark(res->attr);
    frame_mark(res->closure);
}
void op_class_unmark(struct op_class *res) {
    if (res == NULL) return;
    frame_unmark(res->attr);
    frame_unmark(res->closure);
}

void op_class_clear(struct op_class *res) {
    if (res == NULL) return;
    if(res->attr != NULL) frame_free(res->attr);
    if(res->closure != NULL) frame_free(res->closure);
    res->attr = NULL;
    res->closure = NULL;
    res->argument = 0;
    res->class_body = 0;
}
int op_class_cmp(const struct op_class *obj1, const struct op_class *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->class_body != obj2->class_body) return CMP_NEQ;
    return CMP_EQ;
}

void op_class_define(struct op_class *res, size_t class_body, struct parser_st *parser) {
    res->class_body = class_body;
    struct bytecode_st *code = parser->codes.bytecodes[class_body];
    res->argument = code->variable;

    struct frame_st *frame = res->closure = frame_new();
    struct variable_list_st data = parser->closures.closures[code->closure]->data;

    variable_list_set(&frame->attrib, &parser->closures.closures[code->closure]->attrib);
    list_resize(&frame->data, data.size);
    for (size_t i = 0, size = data.size; i < size; i++) {
        frame->data.data[i] = object_copy_obj(parser->var_stack->data[parser->var_start_pos + data.variables[i]->position]);
    }
}
struct object_st *op_class_save_value(struct op_class *res, struct parser_st* parser) {
    struct frame_st *frame = res->attr = frame_new();
    struct variable_list_st* attrib = parser->variables.variable_lists[res->argument];
    struct object_st *result = NULL;
    size_t var_start_pos = parser->var_start_pos;

    variable_list_set(&frame->attrib, attrib);
    list_resize(&frame->data, attrib->size);
    for (size_t i = 0, size = attrib->size; i < size; i++) {
        frame->data.data[i] = object_copy_obj(parser->var_stack->data[var_start_pos + attrib->variables[i]->position]);
        if (frame->attrib.variables[i]->name.size == 8 && memcmp(frame->attrib.variables[i]->name.data, "__init__", 8) == 0)
            result = object_copy_obj(frame->data.data[i]);
    }
    return result;
}

// TLV Methods
int op_class_set_tlv(struct op_class *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    op_class_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_OP_CLASS) return ERR_TLV_TAG;

    res->attr = frame_new();
    res->closure = frame_new();
    struct integer_st position;
    integer_data_init(&position);

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = frame_set_tlv(res->attr, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = frame_set_tlv(res->closure, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->argument = integer_get_si(&position);

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->class_body = integer_get_si(&position);

    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
    return result;
}
void op_class_get_tlv(const struct op_class *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct integer_st position;
    integer_data_init(&position);
    struct string_st _tlv_data = {NULL, 0, 0};
    frame_get_tlv(res->attr, tlv);

    frame_get_tlv(res->closure, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->argument);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->class_body);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_OP_CLASS, tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
}

// Sub method
struct object_st *op_class_subscript(struct error_st *err, struct op_class *obj, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct frame_st *frame = obj->attr;
    if (obj2 == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    struct object_st *res = NULL;
    if (obj2->type != STRING_TYPE) {
        struct object_st *temp = object_new();
        object__int(temp, err, obj2);

        if (err == NULL || !err->present) {
            for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
                if (string_cmp(&frame->attrib.variables[i]->name, temp->data) == 0)
                    res = object_copy_obj(frame->data.data[i]);
            }
            if (res == NULL) {
                string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, temp->data);
                list_resize(&frame->data, frame->data.size + 1);
                frame->data.data[frame->data.size - 1] = object_new();
            }
        }
        object_free(temp);
        return res;
    }
    for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
        if (string_cmp(&frame->attrib.variables[i]->name, obj2->data) == 0)
            res = object_copy_obj(frame->data.data[i]);
    }
    if (res == NULL) {
        string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, obj2->data);
        list_resize(&frame->data, frame->data.size + 1);
        frame->data.data[frame->data.size - 1] = object_new();
    }
    return res;
}
struct object_st *op_class_attrib(struct error_st *err, const struct op_class *obj, const struct string_st *str) {
    struct frame_st *frame = obj->attr;
    struct object_st *res = NULL;
    for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
        if (string_cmp(&frame->attrib.variables[i]->name, str) == 0)
            res = object_copy_obj(frame->data.data[i]);
    }
    if (res == NULL) {
        string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, str);
        list_resize(&frame->data, frame->data.size + 1);
        frame->data.data[frame->data.size - 1] = object_new();
    }
    return res;
}