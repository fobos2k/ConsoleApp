cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ConsoleApp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

find_program(CLANG_BIN clang)
find_program(CLANGXX_BIN clang++)

if (CLANG_BIN AND CLANGXX_BIN)
    message(STATUS "Found Clang: using clang/clang++")
    set(CMAKE_C_COMPILER "${CLANG_BIN}")
    set(CMAKE_CXX_COMPILER "${CLANGXX_BIN}")

    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
else()
    message(WARNING "Clang not found, using default compiler")
endif()

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
