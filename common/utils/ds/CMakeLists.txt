add_library(ds STATIC
            byte_array.c
            assoc_container/assoc_rb_tree.c
            assoc_container/bimap.c
            seq_container/seq_arr.c
)
target_include_directories(ds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
