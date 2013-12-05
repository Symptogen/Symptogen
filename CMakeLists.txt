cmake_minimum_required(VERSION 2.8)

PROJECT(Symptogen)

# Diplay commands being ran by CMake
#set(CMAKE_VERBOSE_MAKEFILE ON)

# IndieLib library
include_directories(${INDIELIB_INCLUDE_DIR})

# OpenGl library
find_package(OpenGL REQUIRED)

# -------------- DEPENDENCIES ---------------------- #
if(UNIX AND NOT APPLE)
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
elseif(WIN32)
	file(GLOB_RECURSE BIN_FILES ${INDIELIB_BINARIES_DIR}/*.dll ${INDIELIB_BINARIES_DIR}/*.lib)
	file(COPY ${BIN_FILES} DESTINATION /)
endif()

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# ------ Sources files ----------- #
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

# Pour Windows on ignore les fichiers linux et vice-versa
if(WIN32)
	list(REMOVE_ITEM SRC_FILES src/CIndieLib.cpp src/CIndieLib.h)
elseif(UNIX AND NOT APPLE)
	list(REMOVE_ITEM SRC_FILES src/CIndieLib_vc2008.cpp src/CIndieLib_vc2008.h)
endif()

# ------------ GDB debug options ----------------- #
set(CMAKE_CXX_FLAGS "-Wall -g")

# ------------ Makefile generation -----------------#
add_executable(Symptogen ${SRC_FILES})
if(UNIX)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY}  glew freeimage ${OPENGL_LIBRARIES} SDL2)
elseif(WIN32)
	set(INDIELIB_LIBRARY 
	${INDIELIB_BINARIES_DIR}/IndieLib.dll 
	${INDIELIB_BINARIES_DIR}/IndieLib.lib
	${INDIELIB_BINARIES_DIR}/IndieLibd.dll 
	${INDIELIB_BINARIES_DIR}/IndieLibd.lib)
	set(SDL2_LIBRARY ${INDIELIB_BINARIES_DIR}/SDL2.dll)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})
endif()
