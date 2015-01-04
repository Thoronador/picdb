# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(picdbd)

#add_definitions(-DYOUR_DEFINE_NAME_HERE)

set(picdbd_sources
    ../../libthoro/common/DirectoryFileList.cpp
    ../../libthoro/common/StringUtils.cpp
    ../../libthoro/hashfunctions/sha-256.cpp
    ../../libthoro/hashfunctions/sha-256_sources.cpp
    ../common/escaping.cpp
    ../common/Query.cpp
    ../common/Splitter.cpp
    ../common/WhoStatRecord.cpp
    ../common/filesystem/functions.cpp
    ../data/Database.cpp
    ../data/DirectDatabase.cpp
    ../data/DatabaseManager.cpp
    ../data/PicData.cpp
    ../sockets/UnixDomainSocketServer.cpp
    ../sockets/picdbvUDSServer.cpp
    Daemonic.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(picdbd ${picdbd_sources})


# find_package (GLUT)
# if (GLUT_FOUND)
#   include_directories(${GLUT_INCLUDE_DIRS})
#   target_link_libraries (picdbd ${GLUT_LIBRARIES})
# else ()
#   message ( FATAL_ERROR "GLUT was not found!" )
# endif (GLUT_FOUND)
