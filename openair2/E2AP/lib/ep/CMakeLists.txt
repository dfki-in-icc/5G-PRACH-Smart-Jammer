add_library(e2ap_ep_obj STATIC e2ap_ep.c sctp_msg.c )
target_link_libraries(e2ap_ep_obj PRIVATE sctp alg)
target_link_libraries(e2ap_ep_obj PUBLIC ds)
