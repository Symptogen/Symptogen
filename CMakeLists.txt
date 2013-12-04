cmake_minimum_required(VERSION 2.8)

project(Symptogen)

#Ajout de la librairie indielib
include_directories(libs/include)
add_subdirectory(libs/include)
set_target_properties(indielib PROPERTIES LINKER_LANGUAGE CXX)

#Ajout de la librairie spriterparser
include_directories(libs/SpriterParser)
add_subdirectory(libs/SpriterParser)
set_target_properties(spriterparser PROPERTIES LINKER_LANGUAGE CXX)

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

#Recuperation des fichiers sources
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

#Generation du makefile
add_executable(Symptogen ${SRC_FILES})
target_link_libraries(Symptogen indielib spriterparser)