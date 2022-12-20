if(E2AP_ENCODING STREQUAL "ASN")
  add_library(e2ap_msg_dec_obj OBJECT e2ap_msg_dec_asn.c)
  target_link_libraries(e2ap_msg_dec_obj PRIVATE asn1_e2ap)
  target_compile_options(e2ap_msg_dec_obj PRIVATE -DASN_DISABLE_OER_SUPPORT)

elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")
  add_library(e2ap_msg_dec_obj OBJECT e2ap_msg_dec_fb.c)

  target_link_libraries(e2ap_msg_dec_obj PRIVATE e2ap_fb_obj)
else()
  message(FATAL_ERROR "no E2AP encoding scheme selected")
endif()

add_compile_definitions(PRIVATE ${E2AP_ENCODING})

target_link_libraries(e2ap_msg_dec_obj PRIVATE e2ap_types_obj)
