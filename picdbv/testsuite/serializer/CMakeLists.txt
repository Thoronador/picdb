cmake_minimum_required (VERSION 3.8)

project(test_serializer)

set(test_serializer_sources
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../common/Splitter.cpp
    ../../data/PicData.cpp
    ../../data/Serializer.cpp
    serializer.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -O3 -Wextra -Wpedantic -Wshadow -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_serializer ${test_serializer_sources})

add_test(NAME class_Serializer COMMAND $<TARGET_FILE:test_serializer>)
