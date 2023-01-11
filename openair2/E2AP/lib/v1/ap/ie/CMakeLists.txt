if(E2AP_ENCODING STREQUAL "ASN")
  include(asn/e2ap-v01.01.cmake)
  add_custom_command(OUTPUT ${e2ap_source} ${e2ap_headers}
    COMMAND ASN1C_PREFIX=E2AP_ asn1c -gen-PER -no-gen-OER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${E2AP_GRAMMAR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${E2AP_GRAMMAR}
    COMMENT "Generating E2AP source files from ${CMAKE_CURRENT_SOURCE_DIR}/${E2AP_GRAMMAR}"
  )

  add_library(asn1_e2ap STATIC ${e2ap_source})
  target_include_directories(asn1_e2ap PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
  target_compile_options(asn1_e2ap PUBLIC -DASN_DISABLE_OER_SUPPORT)
  target_compile_options(asn1_e2ap PRIVATE -Wno-missing-field-initializers -Wno-unused-parameter)
elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")
  add_subdirectory(fb)
  add_library(e2ap_ie_obj INTERFACE)
  target_link_libraries(e2ap_ie_obj INTERFACE  
                        e2ap_fb_obj)
else()
  message(FATAL_ERROR "Unforeseen E2AP_ENCODING type")
endif()



