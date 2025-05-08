find_program(CLANG_BIN clang)
find_program(CLANGXX_BIN clang++)

message(DEBUG "CLANGXX_BIN = ${CLANGXX_BIN}")

if (CLANG_BIN AND CLANGXX_BIN)
    message(STATUS "Found Clang: using clang/clang++")
    set(CMAKE_C_COMPILER "${CLANG_BIN}")
    set(CMAKE_CXX_COMPILER "${CLANGXX_BIN}")

    message(DEBUG "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
else()
    message(WARNING "Clang not found, using default compiler")
endif()
