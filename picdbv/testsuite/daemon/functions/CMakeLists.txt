cmake_minimum_required (VERSION 3.8)

project(extractStatusCode)

set(extractStatusCode_sources
    ../../../../libstriezel/common/StringUtils.cpp
    ../../../daemon/functions.cpp
    extractStatusCode.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -O3 -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(extractStatusCode ${extractStatusCode_sources})

add_test( extractStatusCode ${CMAKE_CURRENT_BINARY_DIR}/extractStatusCode)
