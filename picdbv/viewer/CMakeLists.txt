cmake_minimum_required (VERSION 3.8)

project(picdbv)

add_definitions(-DUSE_GLUT_GUI)

set(picdbv_sources
    ../../libstriezel/common/DirectoryFileList.cpp
    ../../libstriezel/common/IntegerUtils.cpp
    ../../libstriezel/common/StringUtils.cpp
    ../../libstriezel/common/graphics/Bits.cpp
    ../../libstriezel/common/graphics/GLImageStructure.cpp
    ../../libstriezel/common/graphics/GLfunctions.cpp
    ../../libstriezel/common/graphics/ImageLoader.cpp
    ../../libstriezel/common/graphics/gif/DataSubBlock.cpp
    ../../libstriezel/common/graphics/gif/Decoder.cpp
    ../../libstriezel/common/graphics/gif/ElementBase.cpp
    ../../libstriezel/common/graphics/gif/Extensions.cpp
    ../../libstriezel/common/graphics/gif/GIFStructures.cpp
    ../../libstriezel/common/graphics/readBMP.cpp
    ../../libstriezel/common/graphics/readGIF.cpp
    ../../libstriezel/common/graphics/readJPEG.cpp
    ../../libstriezel/common/graphics/readPNG.cpp
    ../../libstriezel/common/graphics/readPPM.cpp
    ../../libstriezel/common/gui/glutGUI.cpp
    ../../libstriezel/filesystem/directory.cpp
    ../../libstriezel/filesystem/file.cpp
    ../../libstriezel/hash/sha256/FileSource.cpp
    ../../libstriezel/hash/sha256/FileSourceUtility.cpp
    ../../libstriezel/hash/sha256/MessageSource.cpp
    ../../libstriezel/hash/sha256/sha256.cpp
    Configuration.cpp
    GUI.cpp
    GUIMultiLinePanel.cpp
    GUITextPanel.cpp
    ../common/Query.cpp
    ../common/Splitter.cpp
    ../common/WhoStatRecord.cpp
    ../data/Database.cpp
    ../data/DirectDatabase.cpp
    ../data/PicData.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-O3 -Wall -Wextra -Wpedantic -Wshadow -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(picdbv ${picdbv_sources})

# libjpeg for reading JPEG images
find_package (JPEG)
if (JPEG_FOUND)
  include_directories(${JPEG_INCLUDE_DIR})
  target_link_libraries (picdbv ${JPEG_LIBRARIES})
else ()
  message ( FATAL_ERROR "libjpeg was not found!" )
endif (JPEG_FOUND)

# libpng for reading PNG images
find_package (PNG)
if (PNG_FOUND)
  include_directories(${PNG_INCLUDE_DIRS})
  target_link_libraries (picdbv ${PNG_LIBRARIES})
else ()
  message ( FATAL_ERROR "libPNG was not found!" )
endif (PNG_FOUND)

# OpenGL for the "graphics".
find_package (OpenGL)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries (picdbv ${OPENGL_LIBRARIES})
else ()
  message ( FATAL_ERROR "OpenGL was not found!" )
endif (OPENGL_FOUND)

# We need to find GLUT for the GUI.
find_package (GLUT)
if (GLUT_FOUND)
  include_directories(${GLUT_INCLUDE_DIRS})
  target_link_libraries (picdbv ${GLUT_LIBRARIES})
else ()
  message ( FATAL_ERROR "GLUT was not found!" )
endif (GLUT_FOUND)
