cmake_minimum_required (VERSION 3.8)

project(picdbd)

set(picdbd_sources
    ../../libstriezel/common/DirectoryFileList.cpp
    ../../libstriezel/common/StringUtils.cpp
    ../../libstriezel/filesystem/directory.cpp
    ../../libstriezel/filesystem/file.cpp
    ../../libstriezel/hash/sha256/FileSource.cpp
    ../../libstriezel/hash/sha256/FileSourceUtility.cpp
    ../../libstriezel/hash/sha256/MessageSource.cpp
    ../../libstriezel/hash/sha256/sha256.cpp
    ../common/escaping.cpp
    ../common/Query.cpp
    ../common/Splitter.cpp
    ../common/WhoStatRecord.cpp
    ../common/filesystem/functions.cpp
    ../data/Database.cpp
    ../data/DirectDatabase.cpp
    ../data/DatabaseManager.cpp
    ../data/PicData.cpp
    ../data/Serializer.cpp
    ../sockets/UnixDomainSocketServer.cpp
    ../sockets/picdbvUDSServer.cpp
    Daemonic.cpp
    commands/CmdAddFile.cpp
    commands/CmdAutoTagDB.cpp
    commands/CmdClearDB.cpp
    commands/CmdCreateDB.cpp
    commands/CmdDeleteDB.cpp
    commands/CmdDeleteFile.cpp
    commands/CmdExistsDB.cpp
    commands/CmdFileData.cpp
    commands/CmdFileExists.cpp
    commands/CmdFilesForHash.cpp
    commands/CmdFilesFromDirectory.cpp
    commands/CmdHashData.cpp
    commands/CmdHashExists.cpp
    commands/CmdHashUnhashed.cpp
    commands/CmdListDatabases.cpp
    commands/CmdListFiles.cpp
    commands/CmdLoadDB.cpp
    commands/CmdNonExistingFiles.cpp
    commands/CmdNumEntries.cpp
    commands/CmdNumHashed.cpp
    commands/CmdNumUnhashed.cpp
    commands/CmdPurge.cpp
    commands/CmdQuery.cpp
    commands/CmdSaveDB.cpp
    commands/CmdTagStatistics.cpp
    commands/CmdUnknownArtist.cpp
    commands/CmdUnknownWho.cpp
    commands/CmdUntagged.cpp
    commands/CmdVersion.cpp
    commands/CmdWhoStatistics.cpp
    commands/Command.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-O3 -Wall -Wextra -Wpedantic -Wshadow -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(picdbd ${picdbd_sources})
