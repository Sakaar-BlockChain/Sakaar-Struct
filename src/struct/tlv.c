#include <stdio.h>
#include "struct.h"

struct object_tlv tlv_tlv = {METHOD_GET_TLV &tlv_get_tlv, METHOD_SET_TLV &tlv_set_tlv};
struct object_type tlv_type = {TLV_OP, &tlv_tlv};

// Class Methods
void tlv_set_str(struct string_st *res, unsigned tag, const char *data, size_t size) {
    string_clear(res);
    unsigned char *size_str = malloc(256);
    size_t size_len = 0;
    unsigned _tag = tag;
    size_t _size = size;

    while (_size > 0) {
        size_str[size_len++] = (unsigned char) (_size & 0xff);
        _size >>= 8;
    }
    if (size > 127) {
        size_str[size_len] = size_len | 0x80;
        size_len++;
    } else if (size) {
        size_len = 1;
    } else {
        size_str[0] = 0;
        size_len = 1;
    }
    while (_tag > 0) {
        size_str[size_len++] = (unsigned char) (_tag & 0xff);
        _tag >>= 8;
    }

    string_resize(res, size_len + size);

    memcpy(res->data + size_len, data, size);
    for (size_t i = 0; i < size_len; i++)
        res->data[i] = (char) size_str[size_len - i - 1];

    free(size_str);
}
void tlv_set_string(struct string_st *res, unsigned tag, const struct string_st *str) {
    if (res == NULL) return;
    if (str == NULL) return string_clear(res);

    unsigned char *size_str = malloc(256);
    size_t size_len = 0;
    unsigned _tag = tag;
    size_t _size = str->size;

    while (_size > 0) {
        size_str[size_len++] = (unsigned char) (_size & 0xff);
        _size >>= 8;
    }
    if (str->size > 127) {
        size_str[size_len] = size_len | 0x80;
        size_len++;
    } else if (str->size) {
        size_len = 1;
    } else {
        size_str[0] = 0;
        size_len = 1;
    }
    while (_tag > 0) {
        size_str[size_len++] = (unsigned char) (_tag & 0xff);
        _tag >>= 8;
    }

    string_resize(res, size_len + str->size);

    memcpy(res->data + size_len, str->data, str->size);
    for (size_t i = 0; i < size_len; i++)
        res->data[i] = (char) size_str[size_len - i - 1];

    free(size_str);
}

size_t tlv_get_size_tag(const struct string_st *tlv) {
    if (tlv == NULL || tlv->data == NULL) goto err;
    if ((tlv->data[0] & 0x1f) != 0x1f) return 1;
    size_t _res = 1;
    while (tlv->data[_res++] & 0x80);
    if (_res > tlv->size) goto err;
    return _res;
    err:
    fprintf(stderr, "Error tlv");
    exit(-1);
}
size_t tlv_get_size_head(const struct string_st *tlv) {
    if (tlv == NULL || tlv->data == NULL) goto err;
    size_t tag_size = tlv_get_size_tag(tlv) + 1;
    size_t _size = (unsigned char) tlv->data[tag_size - 1];
    if (_size & 0x80) {
        _size ^= 0x80;
        tag_size += _size;
    }
    if (tag_size > tlv->size) goto err;
    return tag_size;
    err:
    fprintf(stderr, "Error tlv");
    exit(-1);
}
size_t tlv_get_size_value(const struct string_st *tlv) {
    if (tlv == NULL || tlv->data == NULL) goto err;
    size_t tag_size = tlv_get_size_tag(tlv);
    size_t _size = (unsigned char) tlv->data[tag_size];
    if (_size & 0x80) {
        size_t size = 0;
        _size ^= 0x80;
        if (_size > tlv->size) goto err;
        for (size_t i = tag_size + 1, j = 0; j < _size; i++, j++) {
            size <<= 8;
            size += (unsigned char) tlv->data[i];
        }
        if (size > tlv->size) goto err;
        return size;
    }
    if (_size > tlv->size) goto err;
    return _size;
    err:
    fprintf(stderr, "Error tlv");
    exit(-1);
}

int32_t tlv_get_tag(const struct string_st *tlv) {
    size_t tag_size = tlv_get_size_tag(tlv);
    int _res = 0;
    for (size_t i = 0; i < tag_size; i++) {
        _res <<= 8;
        _res += (unsigned char) tlv->data[i];
    }
    return _res;
}
int8_t tlv_get_value(const struct string_st *tlv, struct string_st *res) {
    size_t head_size = tlv_get_size_head(tlv);
    size_t value_size = tlv_get_size_value(tlv);
    string_set_str(res, tlv->data + head_size, value_size);
    return ERR_SUCCESS;
}

int8_t tlv_get_next_tlv(struct string_st *tlv, struct string_st *res) {
    size_t size = tlv_get_size_head(tlv) + tlv_get_size_value(tlv);
    string_set_str(res, tlv->data, size);

    for (size_t i = size, j = 0, l = tlv->size; i < l; i++, j++)
        tlv->data[j] = tlv->data[i];
    string_resize(tlv, tlv->size - size);
    return ERR_SUCCESS;

}
void tlv_beautify(const struct string_st *tlv, struct string_st *res) {
    if (res == NULL) return;
    if (string_is_null(tlv)) return string_clear(res);
    string_resize(res, tlv->size * 2);
    for (size_t i = 0, size = tlv->size; i < size; i++) {
        res->data[i * 2] = get_char_16((unsigned char) tlv->data[i] / 16);
        res->data[i * 2 + 1] = get_char_16((unsigned char) tlv->data[i] % 16);
    }
}

void tlv_from_beautify(const struct string_st *beaut, struct string_st *res) {
    if (res == NULL) return;
    if (string_is_null(beaut)) return string_clear(res);
    string_resize(res, beaut->size / 2 + beaut->size % 2);
    for (size_t i = 0, size = res->size; i < size; i++)  {
        res->data[i] = (char) (set_char_16(beaut->data[i * 2]) * 16 + set_char_16(beaut->data[i * 2 + 1]));
    }
}

// TLV Methods
int8_t tlv_set_tlv(struct string_st *res, const struct string_st *tlv) {
    string_set(res, tlv);
    return ERR_SUCCESS;
}
void tlv_get_tlv(const struct string_st *res, struct string_st *tlv) {
    string_set(tlv, res);
}