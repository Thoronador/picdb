# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(client_cli)

set(client_cli_sources
    ../common/escaping.cpp
    ../sockets/UnixDomainSocketClient.cpp
    main.cpp)

#if (CMAKE_COMPILER_IS_GNUCC)
#    add_definitions (-Wall -O3 -fexceptions)
#endif (CMAKE_COMPILER_IS_GNUCC)

#set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(client_cli ${client_cli_sources})

