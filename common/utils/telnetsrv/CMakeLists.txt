add_library(telnetsrv MODULE
            telnetsrv.c
            telnetsrv_proccmd.c
            telnetsrv_loader.c
            telnetsrv_measurements.c
)
target_link_libraries(telnetsrv PRIVATE history)
target_link_libraries(telnetsrv PRIVATE asn1_nr_rrc asn1_lte_rrc)

message(STATUS "Add enb specific telnet functions in libtelnetsrv_enb.so")
add_library(telnetsrv_enb MODULE
            telnetsrv_enb_measurements.c
            telnetsrv_enb_phycmd.c
)
add_dependencies(telnetsrv telnetsrv_enb)
target_link_libraries(telnetsrv_enb PRIVATE asn1_nr_rrc asn1_lte_rrc)

message(STATUS "No specific telnet functions for gnb")

message(STATUS "No specific telnet functions for 4Gue")

message(STATUS "Add 5Gue specific telnet functions in libtelnetsrv_5Gue.so")
add_library(telnetsrv_5Gue MODULE telnetsrv_5Gue_measurements.c)
add_dependencies(telnetsrv telnetsrv_5Gue)
target_link_libraries(telnetsrv_5Gue PRIVATE asn1_nr_rrc asn1_lte_rrc)

# all libraries should be written to root build dir
set_target_properties(telnetsrv telnetsrv_enb telnetsrv_5Gue
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../..
)

install(TARGETS telnetsrv telnetsrv_enb telnetsrv_5Gue
        DESTINATION bin
)
