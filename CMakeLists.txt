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

	#FMOD library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/linux/inc")
	file(GLOB_RECURSE FMOD_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/linux/lib/*.so)

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

	#FMOD library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/inc")
	file(GLOB_RECURSE FMOD_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodex_vc.lib
									${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodexL_vc.lib
									${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodex64_vc.lib
									${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodexL64_vc.lib)
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

	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${FMOD_LIBRARY} glew freeimage ${OPENGL_LIBRARIES} SDL2 -lrt)

elseif(WIN32)

	# --------------- Post-build command  -------------- #

	# for IndieLib.dll
	set(INDIELIB_DLL ${INDIELIB_BINARIES_DIR}/IndieLib.dll)
	add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${INDIELIB_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)

    # for IndieLibd.dll
	set(INDIELIBD_DLL ${INDIELIB_BINARIES_DIR}/IndieLibd.dll)
	add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${INDIELIBD_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)

    # for SDL2.dll
	set(SDL2_DLL ${INDIELIB_BINARIES_DIR}/SDL2.dll)
	add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)
  
    # for fmod
    set(FMOD_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/fmodex.dll)
    add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMOD_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)

    set(FMODL_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/fmodexL.dll)
    add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMODL_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)

    set(FMOD64_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/fmodex64.dll)
    add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMOD64_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)

    set(FMODL64_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/fmodexL64.dll)
    add_custom_command(TARGET Symptogen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMODL64_DLL}"
        $<TARGET_FILE_DIR:Symptogen>)
	
	# ----------- Library linking ---------- #
	set(INDIELIB_LIBRARY ${INDIELIB_BINARIES_DIR}/IndieLibd.lib ${INDIELIB_BINARIES_DIR}/IndieLib.lib)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${FMOD_LIBRARY} ${OPENGL_LIBRARIES})

	# --------------- Visual Studio ------------------ #
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_DEBUG "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

endif()

