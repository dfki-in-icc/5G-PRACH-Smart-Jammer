add_library(e2ap_alg_obj OBJECT 
                        alg_ds/alg/defer.c
                        alg_ds/alg/find.c
                        )
target_link_libraries(e2ap_alg_obj PUBLIC ds)

add_library(e2_conf_obj OBJECT 
                        conf_file.c
                        )


add_library(e2_time_obj OBJECT 
                        time_now_us.c
                        )

add_library(e2_ngran_obj OBJECT
                         ngran_type.c
                         )



