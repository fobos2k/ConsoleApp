set(MODULE_NAME "cli")

message(STATUS "===>>> Started: ${MODULE_NAME}...")

add_project_application(${MODULE_NAME})
add_depends_libs(${MODULE_NAME} "common; scenario")

include(cmake/develop.cmake)
print_target_data(lib_${MODULE_NAME})
print_target_data(${MODULE_NAME})

message(STATUS "===>>> Ended: ${MODULE_NAME}...")
