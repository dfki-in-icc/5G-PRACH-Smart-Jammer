add_library(e2_conf_obj OBJECT 
                        conf_file.c
                        )
target_link_libraries(e2_conf_obj PRIVATE alg)


add_library(e2_time_obj OBJECT 
                        time_now_us.c
                        )
