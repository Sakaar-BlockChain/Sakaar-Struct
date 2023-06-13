#include "struct.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
#define PRINT_NEXT(expr) if(expr){printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';

char prefix[100];
void *printing[100];
size_t printing_pos;


void print_int(const struct integer_st *res) {
    printf("integer : ");
#ifdef USE_GMP
    gmp_printf("%Zd", res->mpz_int);
#else
    struct string_st *str = string_new();
    integer_get_str(res, str);
    for(int i=0;i<str->size;i++)printf("%c", str->data[i]);
    string_free(str);
#endif
    printf("\n");
}
void print_str(const struct string_st *res) {
    printf("string (%zu): ", res->size);
    for (int i = 0; i < res->size; i++) printf("%c", res->data[i]);
    printf("\n");
}
void print_tlv(const struct string_st *res) {
    printf("tlv : ");
    for (int i = 0; i < res->size; i++)printf("%.2x ", (unsigned char) res->data[i]);
    printf("\n");
}
void print_list(const struct list_st *res, int size) {
    printf("list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_obj(res->data[i], size + 2);
    }
}
void print_obj(const struct object_st *res, int size) {
    if (res == NULL) {
        printf("None\n");
        return;
    }
    printf("object : (%zu) %p\n", res->counter, res);
    PRINT_PREF
    PRINT_NEXT(0)
    for(size_t i=0;i<printing_pos;i++) {
        if (printing[i] == res) {
            printf("...\n");
            return;
        }
    }
    printing[printing_pos++] = (void *) res;
    if (res->type == NONE_TYPE) printf("None\n");
    else if (res->type == STRING_TYPE) print_str(res->data);
    else if (res->type == TLV_TYPE) print_tlv(res->data);
    else if (res->type == INTEGER_TYPE) print_int(res->data);
    else if (res->type == OBJECT_TYPE) print_obj(res->data, size + 2);
    else if (res->type == LIST_TYPE) print_list(res->data, size + 2);
    printing[--printing_pos] = NULL;
}

int main() {
//    struct object_st *obj = object_new();
//    struct object_st *obj1 = object_new();
//    object_set_type(obj, LIST_TYPE);
//    list_append(obj->data, obj);
//    list_append(obj->data, obj);
//
//    print_obj(obj, 1);
//
//    struct string_st *str = string_new();
//    object_get_tlv(obj, str);
//    print_tlv(str);
//    printf("Stat : %d\n", object_set_tlv(obj1, str));
//    string_free(str);
//
//    print_obj(obj1, 1);
//
//    printf("Pos : %d\n", obj->flag);
//    printf("Pos : %d\n", obj1->flag);
//
//    printf("%zu\n", mem_ctx.filled);
//    object_free(obj);
//    printf("%zu\n", mem_ctx.filled);
//    object_free(obj1);
//    printf("%zu\n", mem_ctx.filled);
    struct object_st *obj = object_new();
    struct object_st *obj1 = object_new();
    object_set_type(obj, LIST_TYPE);
    list_append(obj->data, obj);
    list_append(obj->data, obj);

    object_free(obj);
    object_free(obj1);
    printf("%zu\n", mem_ctx.filled);
}