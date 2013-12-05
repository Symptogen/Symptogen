cmake_minimum_required(VERSION 2.8)

project(Symptogen)

# Diplay commands being ran by CMake
#set(CMAKE_VERBOSE_MAKEFILE ON)

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

# Find all source files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

# Optionsvor compilation
# -Wall => display all warning
# -g => can debug with gdb
set(CMAKE_CXX_FLAGS "-Wall -g")

# Generation of the compilation tools (makefile...)
add_executable(Symptogen ${SRC_FILES})
target_link_libraries(Symptogen ${INDIELIB_LIBRARY}  glew freeimage ${OPENGL_LIBRARIES} SDL2)
