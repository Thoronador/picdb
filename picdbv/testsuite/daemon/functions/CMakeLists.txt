# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(extractStatusCode)

set(extractStatusCode_sources
    ../../../../libstriezel/common/StringUtils.cpp
    ../../../daemon/functions.cpp
    extractStatusCode.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(extractStatusCode ${extractStatusCode_sources})

add_test( extractStatusCode ${CMAKE_CURRENT_BINARY_DIR}/extractStatusCode)
