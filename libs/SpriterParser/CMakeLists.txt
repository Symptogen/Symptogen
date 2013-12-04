cmake_minimum_required(VERSION 2.6)

project(spriterparser)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

file(
    GLOB_RECURSE
    src_files
    *.h 
    *.cpp
)

add_library(spriterparser STATIC ${src_files})

