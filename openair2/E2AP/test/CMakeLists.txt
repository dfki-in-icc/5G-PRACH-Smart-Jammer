####################################
# E2AP Encoding and Decoding tests
###################################

if(E2AP_ENCODING STREQUAL "ASN")

####################
# E2AP ASN Encoding
####################

  add_executable(test_e2ap_encode_asn test_e2ap_encode_asn.c)
  target_link_libraries(test_e2ap_encode_asn
                        PRIVATE
                        # This is wrong. Fix It
                        e2_agent
                        asn1_e2ap
                        #$<TARGET_OBJECTS:e2ap_ie_obj>
                        )
  target_compile_options(test_e2ap_encode_asn  PRIVATE -Wno-missing-field-initializers -Wno-unused-parameter)
  enable_testing()
  add_test(e2ap_test_encode_asn test_e2ap_encode_asn)

  add_executable(test_e2ap_enc_dec_asn test_e2ap_enc_dec_asn.c)
  target_link_libraries(test_e2ap_enc_dec_asn
                        PUBLIC
                        # This is wrong. Fix It
                        e2_agent
                        asn1_e2ap
                        #$<TARGET_OBJECTS:e2ap_ie_obj>
                        )
  target_compile_options(test_e2ap_enc_dec_asn PRIVATE -Wno-missing-field-initializers -Wno-unused-parameter)
  enable_testing()
  add_test(e2ap_test_enc_dec_asn test_e2ap_enc_dec_asn)

###########################
# E2AP Flatbuffers Encoding
###########################

elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")

  add_executable(test_e2ap_enc_dec_fb test_e2ap_enc_dec_fb.c)

  target_link_libraries(test_e2ap_enc_dec_fb
                      PUBLIC
                      # This is wrong. Fix It
                      e2_agent
                      ${FlatCC}
                      )
  target_compile_definitions(test_e2ap_enc_dec_fb  PRIVATE ${E2AP_ENCODING})
  enable_testing()
  add_test(e2ap_test_enc_dec_fb test_e2ap_enc_dec_fb)

else()
  message(FATAL_ERROR "Unknown E2AP encoding type")
endif()



