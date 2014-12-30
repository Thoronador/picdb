# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(escaping)

#add_definitions(-DUSE_GLUT_GUI)

set(escaping_sources
    ../../common/escaping.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(escaping ${escaping_sources})

add_test( escaping ${CMAKE_CURRENT_BINARY_DIR}/escaping)
