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
    commands/CmdHashData.cpp
    commands/CmdHashExists.cpp
    commands/CmdListDatabases.cpp
    commands/CmdListFiles.cpp
    commands/CmdLoadDB.cpp
    commands/CmdNumEntries.cpp
    commands/CmdNumHashed.cpp
    commands/CmdNumUnhashed.cpp
    commands/CmdQuery.cpp
    commands/CmdUnknownArtist.cpp
    commands/CmdUnknownWho.cpp
    commands/CmdUntagged.cpp
    commands/CmdVersion.cpp
    commands/Command.cpp
    main.cpp)

    add_definitions (-Wall -O3 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(picdbd ${picdbd_sources})
