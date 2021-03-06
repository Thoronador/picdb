# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(picdbd)

#add_definitions(-DYOUR_DEFINE_NAME_HERE)

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

    add_definitions (-Wall -O3 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(picdbd ${picdbd_sources})
