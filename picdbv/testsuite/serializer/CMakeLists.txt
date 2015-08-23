# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test_serializer)

set(test_serializer_sources
    ../../../libthoro/hash/sha256/sha256.cpp
    ../../common/Splitter.cpp
    ../../data/PicData.cpp
    ../../data/Serializer.cpp
    serializer.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_serializer ${test_serializer_sources})

add_test( class_Serializer ${CMAKE_CURRENT_BINARY_DIR}/test_serializer)
