add_subdirectory(e2ap_types)
add_subdirectory(ie)
add_subdirectory(enc)
add_subdirectory(dec)
add_subdirectory(free)


if(E2AP_ENCODING STREQUAL "ASN")
  add_library(e2ap_ap_obj STATIC e2ap_ap_asn.c)

elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")
  add_library(e2ap_ap_obj STATIC e2ap_ap_fb.c)

else()
  message(FATAL_ERROR "Unknown E2AP encoding type")
endif()

# Set the MACRO variable that will be used by the compiler to select the encoding/decoding
target_compile_definitions(e2ap_ap_obj PRIVATE ${E2AP_ENCODING})
target_link_libraries(e2ap_ap_obj PUBLIC
                      e2ap_types_obj
                      e2ap_msg_enc_obj
                      e2ap_msg_dec_obj
                      e2ap_msg_free_obj)
