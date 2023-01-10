add_library(T STATIC T.c local_tracer.c)
#target_include_directories
target_link_libraries(T PUBLIC rt)
target_link_libraries(T PRIVATE CONFIG_LIB)

# This rule is specifically needed to generate T files before anything else in
# a project that uses the T.  See the 'add_dependencies' on some targets.
# Basically we create a custom target and we make other targets depend on it.
# That forces cmake to generate T files before anything else.
add_custom_target (generate_T DEPENDS T_IDs.h)
