set(E1AP_VERSION 16 78 0)
make_version(E1AP_cc ${E1AP_VERSION})
#add_definitions(-DE1AP_VERSION=${E1AP_cc})

# TODO from E1AP_VERSION
set(E1AP_RELEASE "16.78.0")

if(E1AP_RELEASE VERSION_EQUAL "16.78.0")
  include(ASN.1/38463-g80.R16.78.0.cmake)
else()
  message(FATAL_ERROR "unknown E1AP_RELEASE ${E1AP_RELEASE}")
endif()

message(STATUS "E1AP_VERSION ${E1AP_VERSION}")
message(STATUS "E1AP_cc ${E1AP_cc}")
message(STATUS "E1AP_RELEASE ${E1AP_RELEASE}")
message(STATUS "E1AP_GRAMMAR ${E1AP_GRAMMAR}")

add_custom_command(OUTPUT ${e1ap_source} ${e1ap_headers}
  COMMAND ASN1C_PREFIX=E1AP_ asn1c -gen-PER -no-gen-OER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${E1AP_GRAMMAR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${E1AP_GRAMMAR}
  COMMENT "Generating E1AP source files from ${CMAKE_CURRENT_SOURCE_DIR}/${E1AP_GRAMMAR}"
)

add_library(ASN1_E1AP_LIB ${e1ap_source})
target_include_directories(ASN1_E1AP_LIB PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_options(ASN1_E1AP_LIB PRIVATE -DASN_DISABLE_OER_SUPPORT -w)
