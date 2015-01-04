# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(picdbv)

add_definitions(-DUSE_GLUT_GUI)

set(picdbv_sources
    ../libthoro/common/DirectoryFileList.cpp
    ../libthoro/common/IntegerUtils.cpp
    ../libthoro/common/StringUtils.cpp
    ../libthoro/common/graphics/Bits.cpp
    ../libthoro/common/graphics/GLImageStructure.cpp
    ../libthoro/common/graphics/GLfunctions.cpp
    ../libthoro/common/graphics/ImageLoader.cpp
    ../libthoro/common/graphics/gif/DataSubBlock.cpp
    ../libthoro/common/graphics/gif/Decoder.cpp
    ../libthoro/common/graphics/gif/ElementBase.cpp
    ../libthoro/common/graphics/gif/Extensions.cpp
    ../libthoro/common/graphics/gif/GIFStructures.cpp
    ../libthoro/common/graphics/readBMP.cpp
    ../libthoro/common/graphics/readGIF.cpp
    ../libthoro/common/graphics/readJPEG.cpp
    ../libthoro/common/graphics/readPNG.cpp
    ../libthoro/common/graphics/readPPM.cpp
    ../libthoro/common/gui/glutGUI.cpp
    ../libthoro/hashfunctions/sha-256.cpp
    ../libthoro/hashfunctions/sha-256_sources.cpp
    Configuration.cpp
    GUI.cpp
    GUIMultiLinePanel.cpp
    GUITextPanel.cpp
    PicDatabase.cpp
    common/Query.cpp
    common/Splitter.cpp
    common/WhoStatRecord.cpp
    data/Database.cpp
    data/DirectDatabase.cpp
    data/PicData.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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
