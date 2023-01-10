#include "enc_dec_f1ap.h"

byte_array_t encode_pdu_f1ap(F1AP_F1AP_PDU_t const* pdu)
{
  assert(pdu != NULL);

//  xer_fprint(stdout, &asn_DEF_F1AP_F1AP_PDU, pdu);
//  fflush(stdout);

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;

  asn_encode_to_new_buffer_result_t res = asn_encode_to_new_buffer(NULL, syntax, &asn_DEF_F1AP_F1AP_PDU, pdu);

  if(res.buffer == NULL || res.result.encoded == -1){
    printf("Failed the encoding in type %s and xml_type = %s\n", res.result.failed_type->name, res.result.failed_type->xml_tag);
    fflush(stdout);
    assert(res.buffer != NULL && "Failed to encode");
  }

  byte_array_t ba = {.len = res.result.encoded, .buf = res.buffer};
  printf("byte_array len = %ld \n",res.result.encoded);

  return ba;
}

F1AP_F1AP_PDU_t decode_pdu_f1ap(byte_array_t ba)
{
  assert(ba.buf != NULL);
  assert(ba.len != 0);

  F1AP_F1AP_PDU_t pdu = {0}; 
  F1AP_F1AP_PDU_t* pdu_ref = &pdu;  
  asn_dec_rval_t const ret = aper_decode(NULL, &asn_DEF_F1AP_F1AP_PDU, (void **)&pdu_ref, ba.buf, ba.len, 0, 0);
  assert( ret.code == RC_OK);

//  xer_fprint(stdout, &asn_DEF_F1AP_F1AP_PDU, &pdu);
//  fflush(stdout);
  
  return pdu;
}




