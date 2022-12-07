#ifndef KPM_ENCRYPTION_ASN_H
#define KPM_ENCRYPTION_ASN_H

#include "../../../util/byte_array.h"
#include "../ie/kpm_data_ie.h"

typedef struct { } kpm_enc_asn_t;

byte_array_t kpm_enc_event_trigger_asn(kpm_event_trigger_t const* event_trigger);

byte_array_t kpm_enc_action_def_asn(kpm_action_def_t const* action_def);

byte_array_t kpm_enc_ind_hdr_asn(kpm_ind_hdr_t const* ind_hdr);

byte_array_t kpm_enc_ind_msg_asn(kpm_ind_msg_t const* ind_msg);

byte_array_t kpm_enc_func_def_asn(kpm_func_def_t const* func_def);
#endif