#ifndef KPM_DECRYPTION_ASN_H
#define KPM_DECRYPTION_ASN_H

#include "../../../util/byte_array.h"
#include "../ie/kpm_data_ie.h"

typedef struct { } kpm_dec_asn_t;

kpm_event_trigger_t kpm_dec_event_trigger_asn(size_t len, uint8_t const ev_tr[len]);
kpm_action_def_t kpm_dec_action_def_asn(size_t len, uint8_t const action_def[len]);
kpm_ind_hdr_t kpm_dec_ind_hdr_asn(size_t len, uint8_t const ind_hdr[len]);
kpm_ind_msg_t kpm_dec_ind_msg_asn(size_t len, uint8_t const ind_msg[len]);
kpm_func_def_t kpm_dec_func_def_asn(size_t len, uint8_t const func_def[len]);
#endif