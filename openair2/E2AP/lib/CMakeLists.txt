
add_subdirectory(v1)
add_subdirectory(ep)
add_subdirectory(msg_hand)

add_library(pending_events_obj OBJECT 
                                pending_events.c 
                                pending_event_ric.c
                                ind_event.c
                                )
target_link_libraries(pending_events_obj PRIVATE e2ap_types_obj e2ap_ap_obj ds)
