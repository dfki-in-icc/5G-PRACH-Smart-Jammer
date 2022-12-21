add_library(alg OBJECT
            defer.c
            find.c
)
target_include_directories(alg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(alg PUBLIC ds)
