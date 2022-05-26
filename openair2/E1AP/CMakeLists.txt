add_subdirectory(MESSAGES)

add_library(E1AP e1ap.c e1ap_common.c)
target_link_libraries(E1AP
                        PUBLIC ASN1_E1AP_LIB
                        PRIVATE UTIL)
target_include_directories(E1AP PUBLIC ${CMAKE_CURRENT_DIR})
