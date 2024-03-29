#include "smart.h"
#ifdef WIN32
#include <stdio.h>
#endif


void parser_clear(struct parser_st *res) {
    if (res == NULL) return;
    string_clear(&res->file_name);
    string_clear(&res->file_path);
    if (res->data_str != NULL) free(res->data_str);
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;
    res->var_start_pos = 0;
    res->scope_type = ScopeType_None;

    error_clear(res->error);

    bytecode_list_clear(&res->codes);
    closure_list_clear(&res->closures);
    node_list_clear(&res->nodes);
    token_list_clear(&res->tokens);
    variable_list_list_clear(&res->variables);

    bytecode_list_clear(&res->codes_stack);
    closure_list_clear(&res->closures_stack);
    variable_list_list_clear(&res->variables_stack);

    list_clear(res->const_objects);
    list_add_new(res->const_objects, NONE_TYPE);
    list_clear(res->temp_stack);
    list_clear(res->var_stack);
}

void parser_data_init(struct parser_st *res) {
    if (res == NULL) return;
    string_data_init(&res->file_name);
    string_data_init(&res->file_path);
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;
    res->var_start_pos = 0;
    res->scope_type = ScopeType_None;

    res->error = error_new();

    bytecode_list_data_init(&res->codes);
    closure_list_data_init(&res->closures);
    node_list_data_init(&res->nodes);
    token_list_data_init(&res->tokens);
    variable_list_list_data_init(&res->variables);
    res->codes.type = 1;
    res->closures.type = 1;
    res->nodes.type = 1;
    res->tokens.type = 1;
    res->variables.type = 1;

    bytecode_list_data_init(&res->codes_stack);
    closure_list_data_init(&res->closures_stack);
    variable_list_list_data_init(&res->variables_stack);

    res->const_objects = list_new();
    list_add_new(res->const_objects, NONE_TYPE);
    res->temp_stack = list_new();
    res->var_stack = list_new();
}
void parser_data_free(struct parser_st *res) {
    if (res == NULL) return;
    string_data_free(&res->file_name);
    string_data_free(&res->file_path);
    if (res->data_str != NULL) free(res->data_str);

    error_free(res->error);

    bytecode_list_data_free(&res->codes);
    closure_list_data_free(&res->closures);
    node_list_data_free(&res->nodes);
    token_list_data_free(&res->tokens);
    variable_list_list_data_free(&res->variables);

    bytecode_list_data_free(&res->codes_stack);
    closure_list_data_free(&res->closures_stack);
    variable_list_list_data_free(&res->variables_stack);

    list_free(res->const_objects);
    list_free(res->temp_stack);
    list_free(res->var_stack);
}

void parser_set_file(struct parser_st *res, char *file_path) {
    if (res == NULL) return;
    parser_clear(res);

    if (memcmp(file_path + strlen(file_path) - 3, ".sc", 3) != 0)
        return error_set_msg(res->error, ErrorType_Import, "File wrong format");

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
        return error_set_msg(res->error, ErrorType_Import, "File Not Exist");

    fseek(fp, 0, SEEK_END);
    res->data_size = ftell(fp);


    char *name = NULL;
#ifdef WIN32
    (name = strrchr(file_path, '\\')) ? ++name : ((name = strrchr(file_path, '/')) ? ++name : (name = file_path));
#else
    (name = strrchr(file_path, '/')) ? ++name : (name = file_path);
#endif
    printf("name : %s\n",name);
    string_set_str(&res->file_name, name, strlen(name));
    string_set_str(&res->file_path, file_path, strlen(file_path) - strlen(name));




    fseek(fp, 0, SEEK_SET);
    res->data_str = malloc(res->data_size);
#ifdef WIN32
    char c;
    size_t i;
    for (i = 0; i < res->data_size && (c = (char)getc(fp)) != EOF; i++) {
        res->data_str[i] = c;
    }
    res->data_size = i;
#else
    for (size_t i = 0, size = res->data_size; i < size; i++) {
        res->data_str[i] = (char) getc(fp);
    }
#endif
    fclose(fp);
}
void parser_set_str(struct parser_st *res, const struct string_st *str) {
    if (res == NULL) return;
    parser_clear(res);

    res->data_size = str->size;
    res->data_str = malloc(res->data_size);
    memcpy(res->data_str, str->data, str->size);
}
void parser_set_error_token(struct parser_st *parser, char *type, char *msg, size_t token_pos) {
    if (parser == NULL) return;
    error_set_msg(parser->error, type, msg);
    error_set_pos(parser->error,
                     parser->tokens.tokens[token_pos]->line_num,
                     parser->tokens.tokens[token_pos]->line_pos,
                     parser->tokens.tokens[token_pos]->pos);
}


size_t parser_new_ident(struct parser_st *res, struct string_st *name) {
    if (res == NULL) return 0;
    struct variable_list_st *list = variable_list_list_last(&res->variables_stack);
    for (size_t i = 0, size = list->size; i < size; i++) {
        if (string_cmp(&list->variables[i]->name, name) == 0) {
            return i + 1;
        }
    }
    variable_list_add_new(list);
    string_set(&variable_list_last(list)->name, name);
    return list->size;
}
size_t parser_get_ident(struct parser_st *res, struct string_st *name) {
    if (res == NULL) return 0;
    struct variable_list_st *list;
    struct closure_st *closure;

    size_t i = res->variables_stack.size;
    struct variable_st *ptr = NULL;
    size_t pos = -1;

    for (; i > 0; i--) {
        list = res->variables_stack.variable_lists[i - 1];
        for (size_t j = 0, size = list->size; j < size; j++) {
            if (string_cmp(&list->variables[j]->name, name) == 0) {
                ptr = list->variables[j];
                pos = j + 1;
                break;
            }
        }
        if (ptr != NULL) break;
    }

    if (ptr == NULL) return 0;

    for (size_t size = res->variables_stack.size; i < size; i++) {
        if (res->closures_stack.closures[i] == NULL) continue;
        list = res->variables_stack.variable_lists[i];
        closure = res->closures_stack.closures[i];

        variable_list_add_new(list);
        string_set(&variable_list_last(list)->name, name);

        closure_append(closure, variable_list_last(list), ptr);

        ptr = variable_list_last(list);
        pos = list->size;
    }

    return pos;
}
size_t parser_const_obj(struct parser_st *res, struct object_st *obj) {
    if (res == NULL) return 0;
    for(size_t i=0, size = res->const_objects->size; i < size; i++) {
        if (object_cmp(res->const_objects->data[i], obj) == 0) {
            return i;
        }
    }
    list_append(res->const_objects, obj);
    return res->const_objects->size - 1;
}
size_t parser_codespace(struct parser_st *res) {
    if (res == NULL) return 0;
    bytecode_list_add_new(&res->codes);
    return res->codes.size - 1;
}

void parser_store_vars(struct parser_st *res, size_t size, size_t position) {
    if (res == NULL) return;
    list_add_new(res->var_stack, INTEGER_TYPE);
    if (res->var_stack->data[res->var_stack->size-1] == NULL) {
        return error_set_msg(res->error, ErrorType_RunTime, "Memory Over Flow");
    }
    integer_set_ui(res->var_stack->data[res->var_stack->size-1]->data, res->var_start_pos);
    list_add_new(res->var_stack, INTEGER_TYPE);
    if (res->var_stack->data[res->var_stack->size-1] == NULL) {
        return error_set_msg(res->error, ErrorType_RunTime, "Memory Over Flow");
    }
    integer_set_ui(res->var_stack->data[res->var_stack->size-1]->data, position);
    res->var_start_pos = res->var_stack->size - 1;
    list_resize(res->var_stack, res->var_start_pos + 1 + size);
}
size_t parser_restore_vars(struct parser_st *res) {
    if (res == NULL) return 0;
    size_t size = res->var_start_pos;
    size_t position = integer_get_ui(res->var_stack->data[res->var_start_pos]->data);
    res->var_start_pos = integer_get_ui(res->var_stack->data[res->var_start_pos - 1]->data);
    list_resize(res->var_stack, size - 1);
    return position;
}

struct object_st *parser_get_var(struct parser_st *res, struct string_st *name) {
    if (res == NULL || res->variables.size == 0 || res->var_stack->size <= 2) return NULL;
    struct variable_list_st *list = res->variables.variable_lists[0];
    struct object_st *result = NULL;

    for (size_t i = 0, size = list->size; i < size; i++) {
        if (string_cmp(&list->variables[i]->name, name) == 0) {
            result = object_copy_obj(res->var_stack->data[res->var_start_pos + list->variables[i]->position]);
            break;
        }
    }
    return result;
}

// TLV Methods
int8_t parser_set_tlv(struct parser_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    parser_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_PARSER) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = bytecode_list_set_tlv(&res->codes, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = closure_list_set_tlv(&res->closures, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = variable_list_list_set_tlv(&res->variables, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = list_set_tlv(res->const_objects, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = list_set_tlv(res->var_stack, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->var_start_pos, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void parser_get_tlv(const struct parser_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    bytecode_list_get_tlv(&res->codes, tlv);

    closure_list_get_tlv(&res->closures, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    variable_list_list_get_tlv(&res->variables, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    list_get_tlv(res->const_objects, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    list_get_tlv(res->var_stack, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    size_get_tlv(res->var_start_pos, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_PARSER, tlv);
    string_data_free(&_tlv_data);
}