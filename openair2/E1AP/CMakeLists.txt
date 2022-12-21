add_subdirectory(MESSAGES)

add_library(E1AP e1ap.c e1ap_common.c e1ap_api.c)
target_link_libraries(E1AP
                      PUBLIC ASN1_E1AP_LIB
                      PRIVATE asn1_nr_rrc asn1_lte_rrc asn1_f1ap UTIL E1_PDCP_IF E1_IF)
target_include_directories(E1AP PUBLIC ${CMAKE_CURRENT_DIR})
