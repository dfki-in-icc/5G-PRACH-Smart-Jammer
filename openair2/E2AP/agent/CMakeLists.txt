if(E2AP_LIBRARY STREQUAL "STATIC")
  set(E2_AGENT_BLD_LIB  "STATIC")
elseif(E2AP_LIBRARY STREQUAL "DYNAMIC")
  set(E2_AGENT_BLD_LIB  "SHARED")
else()
  message(FATAL_ERROR "Unknown type")
endif()


set(E2_AGENT_SRC 
            asio_agent.c
            e2ap_agent.c
            e2_agent.c
            e2_agent_api.c
            endpoint_agent.c
            msg_handler_agent.c
            e2_agent_api.c
            plugin_agent.c
            not_handler_agent.c
            ../sm/sm_proc_data.c
  )

if(E2AP_ENCODING STREQUAL "ASN")

  add_library(e2_agent STATIC ${E2_AGENT_SRC})
  target_link_libraries(e2_agent PRIVATE asn1_e2ap)

elseif(E2AP_ENCODING STREQUAL "FLATBUFFERS")
          add_library(e2_agent ${E2_AGENT_BLD_LIB} #STATIC or SHARED
            ${E2_AGENT_SRC}
           )

          #bundle_static_library(e2_agent libflatccrt_d.a)



else()
  message(FATAL_ERROR "Error, while compiling the agent" )
endif()


if(INCL_WHAT_YOU_USE)
  find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
  set_property(TARGET e2_agent  PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

if(${E2AP_LIBRARY} STREQUAL "DYNAMIC" )
  target_link_libraries(e2_agent  
                       PUBLIC  
                       -pthread 
                       -lsctp 
                       -ldl 
                       ) 

endif()

target_compile_definitions(e2_agent PRIVATE ${E2AP_ENCODING})
target_link_libraries(e2_agent PUBLIC
                      e2ap_ap_obj
                      e2ap_ep_obj
                      pending_events_obj
                      e2ap_types_obj
                      e2ap_msg_enc_obj
                      e2ap_msg_dec_obj
                      e2ap_msg_free_obj
)
target_link_libraries(e2_agent PRIVATE alg ngran_types)
target_link_libraries(e2_agent PUBLIC ds) # for byte_array_t
