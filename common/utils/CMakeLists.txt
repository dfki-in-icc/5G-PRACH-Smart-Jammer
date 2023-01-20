add_boolean_option(ENABLE_TELNETSRV OFF "Whether to build telnet support in modems")
if(ENABLE_TELNETSRV)
  add_subdirectory(telnetsrv)
endif()

add_boolean_option(ENABLE_WEBSRV OFF "Whether to build the webserver")
if(ENABLE_WEBSRV)
  add_subdirectory(websrv)
endif()
