#include "struct.h"

struct object_type tlv_type = {TLV_OP, METHOD_GET_TLV &tlv_get_tlv, METHOD_SET_TLV &tlv_set_tlv};


void tlv_set_str(struct string_st *res, unsigned tag, const char *data, size_t size) {
    string_clear(res);
    unsigned char *size_str = skr_malloc(256);
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
    } else if (size != 0) {
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

    skr_free(size_str);
}
void tlv_set_string(struct string_st *res, unsigned tag, const struct string_st *str) {
    if (res == NULL) return;
    if (str == NULL) return string_clear(res);

    unsigned char *size_str = skr_malloc(256);
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
    } else if (str->size != 0) {
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

    skr_free(size_str);
}

size_t tlv_get_size_tag(const char *data) {
    if ((data[0] & 0x1f) != 0x1f) return 1;
    size_t _res = 1;
    while (data[_res++] & 0x80);
    return _res;
}
size_t tlv_get_size(char *data) {
    if (data == NULL) return 0;
    size_t tag_size = tlv_get_size_tag(data);
    size_t _size = (unsigned char) data[tag_size];
    if (_size & 0x80) {
        size_t size = 0;
        _size ^= 0x80;
        for (size_t i = tag_size + 1, j = 0; j < _size; i++, j++) {
            size <<= 8;
            size += (unsigned char) data[i];
        }
        return size;
    }
    return _size;
}

unsigned tlv_get_tag(char *data) {
    if (data == NULL) return 0;
    size_t tag_size = tlv_get_size_tag(data);
    unsigned _res = 0;
    for (size_t i = 0; i < tag_size; i++) {
        _res <<= 8;
        _res += (unsigned char) data[i];
    }
    return _res;
}
char *tlv_get_value(char *data) {
    if (data == NULL) return NULL;
    size_t tag_size = tlv_get_size_tag(data);
    size_t _size = (unsigned char) data[tag_size];
    if (_size & 0x80) {
        _size ^= 0x80;
        tag_size += _size;
    }
    tag_size++;
    return data + tag_size;
}

char *tlv_get_next_tlv(char *data, struct string_st *res) {
    if (data == NULL) return NULL;
    size_t _size = tlv_get_size(data) + tlv_get_value(data) - data;
    string_set_str(res, data, _size);
    return data + _size;
}
void tlv_beautify(const struct string_st *tlv, struct string_st *res) {
    if (res == NULL) return;
    if (string_is_null(tlv)) return string_clear(res);
    string_resize(res, tlv->size * 2);
    for (size_t i = 0; i < tlv->size; i++) {
        res->data[i * 2] = get_char_16((unsigned char) tlv->data[i] / 16);
        res->data[i * 2 + 1] = get_char_16((unsigned char) tlv->data[i] % 16);
    }
}

// TLV methods
void tlv_set_tlv(struct string_st *res, const struct string_st *tlv) {
    string_set(res, tlv);
}
void tlv_get_tlv(const struct string_st *res, struct string_st *tlv) {
    string_set(tlv, res);
}