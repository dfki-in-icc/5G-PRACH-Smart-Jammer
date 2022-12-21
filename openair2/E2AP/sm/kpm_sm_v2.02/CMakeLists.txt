cmake_minimum_required(VERSION 3.15) # setting this is required
project(kpm_sm)            # this sets the project name

set(SM_ENCODING_KPM_SRC
                      ../sm_proc_data.c 
                      kpm_sm_ric.c 
                      kpm_sm_agent.c 
                      ie/kpm_data_ie.c
                      )

if(SM_ENCODING_KPM STREQUAL "PLAIN")
  add_library(kpm_sm SHARED
                      ${SM_ENCODING_KPM_SRC}
                      enc/kpm_enc_plain.c 
                      dec/kpm_dec_plain.c 
                      )
elseif(SM_ENCODING_KPM STREQUAL "ASN")
  
  add_subdirectory(ie/asn)
  add_library(kpm_sm SHARED
                      ${SM_ENCODING_KPM_SRC}
                      enc/kpm_enc_asn.c 
                      dec/kpm_dec_asn.c 
                      )
  target_link_libraries(kpm_sm PRIVATE m asn1_e2sm_kpm)

elseif(SM_ENCODING_KPM STREQUAL "FLATBUFFERS" )
  message(FATAL_ERROR "KPM SM FB not implemented")
else()
  message(FATAL_ERROR "Unknown KPM SM encoding type")
endif()

target_compile_definitions(kpm_sm PUBLIC ${SM_ENCODING_KPM})
target_link_libraries(kpm_sm PUBLIC ds)
target_link_libraries(kpm_sm PRIVATE alg)
