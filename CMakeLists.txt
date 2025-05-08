cmake_minimum_required(VERSION 3.21)

include(cmake/clang.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ConsoleApp)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

set(COMMON_WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
)

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

string(TOLOWER ${PROJECT_NAME} EXECUTABLE_NAME)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${INCLUDE_DIR})

message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Applying Clang-specific warnings")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        ${COMMON_WARNING_FLAGS}
        -Wold-style-cast
        -Wnon-virtual-dtor
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Applying GCC-specific warnings")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        ${COMMON_WARNING_FLAGS}
        -Wno-psabi
    )
else()
    message(WARNING "Unknown compiler, applying only common warnings")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        ${COMMON_WARNING_FLAGS}
    )
endif()

if (WIN32)
    include(cmake/windows_setup.cmake)
endif()

