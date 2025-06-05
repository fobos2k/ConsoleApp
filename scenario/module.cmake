set(MODULE_NAME "scenario")

message(STATUS "===>>> Started: ${MODULE_NAME}...")

add_project_library(${MODULE_NAME})
add_depends_libs(${MODULE_NAME} "common")

include(cmake/develop.cmake)
print_target_data(${MODULE_NAME})

message(STATUS "===>>> Ended: ${MODULE_NAME}...")
