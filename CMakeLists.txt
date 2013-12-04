cmake_minimum_required(VERSION 2.8)

project(Symptogen)

# IndieLib library
include_directories(${INDIELIB_INCLUDE_DIR})

# OpenGl library
find_package(OpenGL REQUIRED)

# SDL2 library
include_directories("${INDIELIB_DEPENDENCIES_DIR}/SDL-2.0/include")

# Glew1.9 library
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-1.9.0/include")
add_definitions(-DGLEW_STATIC=1)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-1.9.0")

# TinyXML library
include_directories("${INDIELIB_DEPENDENCIES_DIR}/FreeImage/source")

# SpiterParser library
include_directories("${INDIELIB_DEPENDENCIES_DIR}/SpriterParser")

# FreeImage library
include_directories("${INDIELIB_DEPENDENCIES_DIR}/FreeImage/source" ${CMAKE_CURRENT_SOURCE_DIR}/libs/freeimage)

# TmxParser
include_directories("${INDIELIB_DEPENDENCIES_DIR}/TmxParser")

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Recuperation des fichiers sources
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

# Options de debug gdb
set(CMAKE_CXX_FLAGS "-Wall -g")

# Generation du makefile
add_executable(Symptogen ${SRC_FILES})
target_link_libraries(Symptogen ${INDIELIB_LIBRARY}  glew freeimage ${OPENGL_LIBRARIES} SDL2)