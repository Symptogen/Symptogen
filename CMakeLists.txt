cmake_minimum_required(VERSION 2.8)

project(Symptogen)

#### IndieLib ###
include_directories(${INDIELIB_INCLUDE_DIR})

#### SDL ###
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

#### OPENGL ####


#### GLEW ####

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

#Recuperation des fichiers sources
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

#Options de debug gdb
set(CMAKE_CXX_FLAGS "-Wall -g")

#Generation du makefile
add_executable(Symptogen ${SRC_FILES})
target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${SDL_LIBRARY})