cmake_minimum_required(VERSION 2.6)

project(glew)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(
    GLOB_RECURSE
    src_files
    src/*.c include/*.h
)

add_library(glew STATIC ${src_files})