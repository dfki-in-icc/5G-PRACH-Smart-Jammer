cmake_minimum_required (VERSION 3.0)
project (f1ap C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas  -g -std=gnu11 -fsanitize=address -fno-omit-frame-pointer")


#include_directories(${SOURCE_DIR})
include_directories("/home/mir/workspace/openairinterface5g_nettle/cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/" )
include_directories("/usr/local/share/asn1c/")


file(GLOB SRC
  #    "/home/mir/workspace/oai_f1ap/cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/*.h"
    "/home/mir/workspace/oai_f1ap/cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/*.c"
)


option(CODE_COVERAGE "Code coverage" ON)
if(CODE_COVERAGE)
  add_compile_options("-fprofile-arcs;-ftest-coverage")
  add_link_options("-lgcov;-coverage")
  message(STATUS "Code Coverage ON. Example usage: lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out && cd out && firefox index.html")

  message(STATUS "Code Coverage ON. Example usage: lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out && cd out && firefox index.html")
  message(STATUS "To merge different coverages: lcov --add-tracefile coverage1.info -a coverage2.info ...coverageN -o merged.info")

endif()


add_subdirectory(f1ap_types)

add_executable(f1ap_test  test.c 
                          byte_array.c 
                          defer.c 
                          cp_ir_to_asn.c 
                          cp_asn_to_ir.c 
                          enc_dec_f1ap.c
                          gen_rnd_data.c 
                          $<TARGET_OBJECTS:f1ap_types_obj> ${SRC})

target_compile_options( f1ap_test PUBLIC "-DASN_DISABLE_OER_SUPPORT -DPDU")


