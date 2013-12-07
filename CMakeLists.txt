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
	#file(COPY ${BIN_FILES} DESTINATION /)
endif()

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# ------ Sources files ----------- #
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

# ------------ GDB debug options ----------------- #
set(CMAKE_CXX_FLAGS "-Wall -g")


# ------------ Makefile generation -----------------#
add_executable(Symptogen ${SRC_FILES})

if(UNIX)

	target_link_libraries(Symptogen ${INDIELIB_LIBRARY}  glew freeimage ${OPENGL_LIBRARIES} SDL2 -lrt)

elseif(WIN32)

	# --------------- Post-build command  -------------- #

	#MACRO(MAKE_WINDOWS_PATH pathname)
		# An extra \\ escape is necessary to get a \ through CMake's processing.
		#STRING(REPLACE "/" "\\" ${pathname} "${${pathname}}")
		# Enclose with UNESCAPED quotes.  This means we need to escape our
		# quotes once here, i.e. with \"
		#SET(${pathname} \"${${pathname}}\")
	#ENDMACRO(MAKE_WINDOWS_PATH)

	# for IndieLib.dll
	#set(INDIELIB_DLL ${INDIELIB_BINARIES_DIR}/IndieLib.dll)
	#MAKE_WINDOWS_PATH(${INDIELIB_DLL})
	#add_custom_command(TARGET Symptogen POST_BUILD COMMAND COPY ${INDIELIB_DLL} ${INDIELIB_BINARIES_DIR}/$<CONFIGURATION>)
	
	# ----------- Library linking ---------- #
	set(INDIELIB_LIBRARY ${INDIELIB_BINARIES_DIR}/IndieLibd.lib ${INDIELIB_BINARIES_DIR}/IndieLib.lib)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${OPENGL_LIBRARIES})

	# --------------- Visual Studio ------------------ #
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_DEBUG "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

endif()

