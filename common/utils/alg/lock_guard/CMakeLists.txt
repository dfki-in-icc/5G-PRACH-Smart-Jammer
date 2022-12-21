cmake_minimum_required(VERSION 3.5)
project(lock_guard)

set(default_build_type "Debug")

set(SANITIZER "ADDRESS" CACHE STRING "Sanitizers")
set_property(CACHE SANITIZER PROPERTY STRINGS "NONE" "ADDRESS" "THREAD")
message(STATUS "Selected SANITIZER TYPE: ${SANITIZER}")

if(SANITIZER STREQUAL "ADDRESS")
  add_compile_options("-fno-omit-frame-pointer;-fsanitize=address;-Wall;-Werror;-g")
add_link_options("-fsanitize=address")

elseif(SANITIZER STREQUAL  "THREAD" )

add_compile_options("-fsanitize=thread;-g;")
add_link_options("-fsanitize=thread;")

endif()

option(CODE_COVERAGE "Code coverage" ON)
if(CODE_COVERAGE)
  add_compile_options("-fprofile-arcs;-ftest-coverage")
  add_link_options("-lgcov;-coverage;")
  message("Code Coverage cmd: cd CMakeFiles/tc.dir && lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out && cd out && firefox index.html")
endif()

option(CODE_PROFILER "Code Profiler" ON)
if(CODE_PROFILER)
  add_compile_options("-pg")
  add_link_options("-pg")
  message("Code Profiler cmd: gprof tc gmon.out > analysis.txt && vim analysis.txt  ")
endif()

add_executable(test_lock_guard test.c)
target_link_libraries(test_lock_guard PRIVATE pthread alg)
