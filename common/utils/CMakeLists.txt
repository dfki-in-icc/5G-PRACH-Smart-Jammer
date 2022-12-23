add_boolean_option(ENABLE_WEBSRV OFF "Whether to build the webserver")
if(ENABLE_WEBSRV)
  add_subdirectory(websrv)
endif()
