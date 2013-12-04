cmake_minimum_required(VERSION 2.8)

project(Symptogen)

#### IndieLib ###
set(INDIELIB_INCLUDE_DIR "libs/include")
set(INDIELIB_LIBRARY "")
include_directories(${INDIELIB_INCLUDE_DIR})
message("INDIELIB_INCLUDE_DIR : "${INDIELIB_INCLUDE_DIR})

#### SDL ###
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
message("SDL_INCLUDE_DIR : "${SDL_INCLUDE_DIR})

#### OPENGL ####
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
message("OPENGL_INCLUDE_DIR : "${OPENGL_INCLUDE_DIR})

#### GLEW ####


# Find all sources
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

# options for compilation
# -Wall => display warnings
# -g => debug gdb
set(CMAKE_CXX_FLAGS "-Wall -g")
# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Generate tools to compile and link on your environment
add_executable(Symptogen ${SRC_FILES})
target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARY})