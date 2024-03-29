#include "struct.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
#define PRINT_NEXT(expr) if (expr) {printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';

char prefix[100];
void *printing[100];
size_t printing_pos;


void print_int(const struct integer_st *res) {
    printf("integer : ");
#ifdef USE_GMP
    gmp_printf("%Zd", res->mpz_int);
#else
    struct string_st str;
    string_data_init(&str);

    integer_get_str(res, &str);
    for(int i=0;i<str.size;i++)printf("%c", str.data[i]);
    string_data_free(&str);
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
}