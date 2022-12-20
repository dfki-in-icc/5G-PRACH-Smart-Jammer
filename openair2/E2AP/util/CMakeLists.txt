
add_library(e2ap_ds_obj OBJECT 
                        byte_array.c 
                        alg_ds/ds/seq_container/seq_arr.c
                        alg_ds/ds/assoc_container/assoc_rb_tree.c
                        alg_ds/ds/assoc_container/bimap.c
                        )

add_library(e2ap_alg_obj OBJECT 
                        alg_ds/alg/defer.c
                        alg_ds/alg/find.c
                        $<TARGET_OBJECTS:e2ap_ds_obj>
                        )

add_library(e2_conf_obj OBJECT 
                        conf_file.c
                        )


add_library(e2_time_obj OBJECT 
                        time_now_us.c
                        )

add_library(e2_ngran_obj OBJECT
                         ngran_type.c
                         )



