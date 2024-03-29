#include "basic.h"

struct object_tlv smartcontract_tlv = {METHOD_GET_TLV &smartcontract_get_tlv, METHOD_SET_TLV &smartcontract_set_tlv};
struct object_type smartcontract_type = {SMARTCONTRACT_OP, &smartcontract_tlv};


// TLV Methods
int8_t smartcontract_set_tlv(struct string_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    string_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_SMARTCONTRACT) return ERR_TLV_TAG;
    return tlv_get_value(tlv, res);
}
void smartcontract_get_tlv(const struct string_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    tlv_set_string(tlv, TLV_SMARTCONTRACT, res);
}