cmake_minimum_required(VERSION 2.6)

project(indielib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

file(
    GLOB_RECURSE
    header_files
    *.h
)

add_library(indielib STATIC ${header_files})

