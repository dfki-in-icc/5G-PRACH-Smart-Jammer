find_library(ULFIUS NAMES "libulfius.so" REQUIRED)
if (NOT ULFIUS)
  message(FATAL_ERROR "ulfius library (https://github.com/babelouest/ulfius) not found, install libulfius-dev (ubuntu) if you need to build websrv back-end")
else()
  message(STATUS "found libulfius for websrv")
endif()

find_library(JSON NAMES "libjansson.so" REQUIRED)
if (NOT JSON)
  message(FATAL_ERROR "libjansson not found, install libjansson-dev for ubuntu, jansson-devel for fedora if you need to build websrv back-end")
else()
  message(STATUS "found libjansson for websrv")
endif()

find_program(NPM NAMES npm REQUIRED)
if (NOT NPM)
  message(FATAL_ERROR "npm is not installed, frontend can't be built. Possibly install npm, package is available for ubuntu and fedora")
else()
  message(STATUS "found npm for websrv")
endif()


# build the backend ( the embedded web server)

message(STATUS "Configuring webserver backend")
add_library(websrv MODULE
            websrv.c
            websrv_websockets.c
            websrv_scope.c
            websrv_noforms.c
            websrv_scope.c
            websrv_utils.c
            ../../../openair1/PHY/TOOLS/nr_phy_scope.c
)
target_link_libraries(websrv PRIVATE asn1_lte_rrc asn1_nr_rrc)
target_link_libraries(websrv PRIVATE ulfius jansson)
target_compile_definitions(websrv PUBLIC WEBSRVSCOPE)
# put websrv into build root and not under build/common/utils/websrv/
set_target_properties(websrv PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../..)

# build the frontend ( loaded from web server by browsers)

add_custom_target(
  websrvfront_installjsdep
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/frontend
  COMMAND npm install
  DEPENDS frontend/package-lock.json
)

message(STATUS "Configuring webserver frontend")
add_custom_target(
  websrvfront
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/frontend
  COMMAND npm run build
  #COMMAND npm run builddev
  DEPENDS websrvfront_installjsdep
)

# copy frontend files to a websrv subdirectory of where oai copy all built files
add_custom_command(TARGET websrvfront
                   POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/frontend/dist/softmodem-ngx ${OPENAIR_BIN_DIR}/../websrv
                              COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/helpfiles ${OPENAIR_BIN_DIR}/../websrv/helpfiles
                   COMMENT "Moving frontend files to:\n   ${OPENAIR_BIN_DIR}/../websrv \n " )
