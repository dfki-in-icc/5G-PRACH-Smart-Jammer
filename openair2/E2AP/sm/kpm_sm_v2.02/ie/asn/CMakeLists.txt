include(e2sm-kpmv2.02.cmake)

add_custom_command(OUTPUT ${e2sm_kpm_source} ${e2sm_kpm_headers}
  COMMAND ASN1C_PREFIX=E2SM_KPMv2_ asn1c -gen-PER -no-gen-OER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${E2SM_KPMv2_GRAMMAR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${E2SM_KPMv2_GRAMMAR}
  COMMENT "Generating E2SM-KPM source files from ${CMAKE_CURRENT_SOURCE_DIR}/${E2SM_KPMv2_GRAMMAR}"
)

add_library(asn1_e2sm_kpm STATIC ${e2sm_kpm_source})
target_include_directories(asn1_e2sm_kpm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_compile_options(asn1_e2sm_kpm PUBLIC -DASN_DISABLE_OER_SUPPORT)
target_compile_options(asn1_e2sm_kpm PRIVATE -Wno-missing-field-initializers -Wno-unused-parameter)
