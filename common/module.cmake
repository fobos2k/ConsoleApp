set(MODULE_NAME "common")

message(STATUS "===>>> Started: ${MODULE_NAME}...")

add_project_library(${MODULE_NAME})

include(cmake/develop.cmake)
print_target_data(${MODULE_NAME})

message(STATUS "===>>> Ended: ${MODULE_NAME}...")
