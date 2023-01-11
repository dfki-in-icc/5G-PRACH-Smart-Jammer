if(E2AP_ENCODING STREQUAL "ASN")
  add_library(e2ap_msg_enc_obj STATIC e2ap_msg_enc_asn.c)
  target_link_libraries(e2ap_msg_enc_obj PRIVATE asn1_e2ap)

elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")
  add_library(e2ap_msg_enc_obj STATIC e2ap_msg_enc_fb.c)
  target_link_libraries(e2ap_msg_enc_obj PRIVATE e2ap_fb_obj )
else()
  message(FATAL_ERROR "Unknonw E2AP encoding type")
endif()

target_link_libraries(e2ap_msg_enc_obj PUBLIC e2ap_types_obj)
