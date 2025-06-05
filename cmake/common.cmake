string(TOLOWER ${PROJECT_NAME}_cli APP_NAME_BIN_CLI)
string(TOLOWER ${PROJECT_NAME} APP_NAME_BIN_GUI)
string(TOLOWER lib${PROJECT_NAME} APP_NAME_LIB)

find_program(CLANG_BIN clang)
find_program(CLANGXX_BIN clang++)
find_program(CLANGXX_TIDY clang-tidy)

if (CLANG_BIN AND CLANGXX_BIN)
  message(STATUS "Found Clang: using clang/clang++.")
  set(CMAKE_C_COMPILER "${CLANG_BIN}")
  set(CMAKE_CXX_COMPILER "${CLANGXX_BIN}")
  set(CMAKE_CXX_CLANG_TIDY "${CLANGXX_TIDY}")
else()
  message(WARNING "Clang not found, using default compiler.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_SOURCE_DIR src)
set(COMMON_INCLUDE_DIR include)
set(COMMON_DEFAULT_CPP_EXT .cpp)
set(COMMON_DEFAULT_HPP_EXT .hpp)

# Compiler flags
set(COMMON_WARNING_FLAGS
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion
  -Wshadow
)

set(CLANG_WARNING_FLAGS
  -Wold-style-cast
  -Wnon-virtual-dtor
)

set(GNU_WARNING_FLAGS
  -Wno-psabi
)

set(COMMON_WIN32_DEFINITIONS
  WIN32_LEAN_AND_MEAN
  NOMINMAX
  _WIN32_WINNT=0x0601
)
