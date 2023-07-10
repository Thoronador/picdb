cmake_minimum_required (VERSION 3.8)

project(escaping)

#add_definitions(-DUSE_GLUT_GUI)

set(escaping_sources
    ../../common/escaping.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -O3 -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(escaping ${escaping_sources})

add_test(NAME escaping COMMAND $<TARGET_FILE:escaping>)
