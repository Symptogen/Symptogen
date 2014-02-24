cmake_minimum_required(VERSION 2.8)

PROJECT(Symptogen)

# Determine whever the os is a 32 bits architecture or a 64 bits one
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(bitness 64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(bitness 32)
endif()

# Diplay commands being ran by CMake
#set(CMAKE_VERBOSE_MAKEFILE ON)

# IndieLib library
include_directories(${INDIELIB_INCLUDE_DIR})

# OpenGl library
find_package(OpenGL REQUIRED)

# -------------- DEPENDENCIES ---------------------- #
if(UNIX AND NOT APPLE)
	# ----- Internal dependencies ----- #

	# SDL2 library
	include_directories("${INDIELIB_DEPENDENCIES_DIR}/SDL-2.0/include")

	# TinyXML2.6 library
	include_directories("${INDIELIB_DEPENDENCIES_DIR}/tinyxml")

	# SpiterParser library
	include_directories("${INDIELIB_DEPENDENCIES_DIR}/SpriterParser")

	# FreeImage library
	include_directories("${INDIELIB_DEPENDENCIES_DIR}/FreeImage/source" ${CMAKE_CURRENT_SOURCE_DIR}/libs/freeimage)

	# TmxParser
	include_directories("${INDIELIB_DEPENDENCIES_DIR}/TmxParser")

	# Glew1.9 library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-1.9.0/include")
	add_definitions(-DGLEW_STATIC=1)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-1.9.0")

	# ----- Extrenal dependencies ----- #

	#FMOD library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/linux/inc")
	if(bitness EQUAL 32)
		file(GLOB_RECURSE FMOD_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/linux/lib/x32/*.so)
	elseif(bitness EQUAL 64)
		file(GLOB_RECURSE FMOD_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/linux/lib/x64/*.so)
	endif()

	# Box2D library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/include")
	if(bitness EQUAL 32)
		file(GLOB_RECURSE BOX2D_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/linux_libs/x32/libBox2D.so)
	elseif(bitness EQUAL 64)
		file(GLOB_RECURSE BOX2D_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/linux_libs/x64/libBox2D.so)
	endif()

elseif(WIN32)
	# for Windows case, all IndieLib was build on a computer, and share to the team
	file(GLOB_RECURSE BIN_FILES ${INDIELIB_BINARIES_DIR}/*.dll ${INDIELIB_BINARIES_DIR}/*.lib)

	# ----- Extrenal dependencies ----- #

	# FMOD library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/inc")
	if(bitness EQUAL 32)
		file(GLOB_RECURSE FMOD_LIBRARY 	${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodex_vc.lib
										${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodexL_vc.lib)
	elseif(bitness EQUAL 64)
		file(GLOB_RECURSE FMOD_LIBRARY 	${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodex64_vc.lib
										${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD-4.44.27/win/lib/fmodexL64_vc.lib)
	endif()

	# Box2D library
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/include")
	if(bitness EQUAL 32)
		file(GLOB_RECURSE BOX2D_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/win_libs/Box2D_d32.lib 
										${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/win_libs/Box2D_r32.lib)
	elseif(bitness EQUAL 64)
		file(GLOB_RECURSE BOX2D_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/win_libs/Box2D_r64.lib 
										${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D_v2.3.0/win_libs/Box2D_d64.lib)
	endif()
endif()


# ------------ Compilation options ------------ #
# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=gnu++0x ${CMAKE_CXX_FLAGS}")

# Enable GDB debug
set(CMAKE_CXX_FLAGS "-Wall -g ${CMAKE_CXX_FLAGS}")

# ------------ Sources files ------------ #
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

# ------------ Create an executable target -----------------#
add_executable(Symptogen ${SRC_FILES})

if(UNIX)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${FMOD_LIBRARY} ${BOX2D_LIBRARY} glew freeimage ${OPENGL_LIBRARIES} SDL2 -lrt)

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
    if(bitness EQUAL 32)
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

    elseif(bitness EQUAL 64)
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
	endif()
	
	# ----------- Library linking ---------- #
	set(INDIELIB_LIBRARY ${INDIELIB_BINARIES_DIR}/IndieLibd.lib ${INDIELIB_BINARIES_DIR}/IndieLib.lib)
	target_link_libraries(Symptogen ${INDIELIB_LIBRARY} ${FMOD_LIBRARY} ${BOX2D_LIBRARY} ${OPENGL_LIBRARIES})

	# --------------- Visual Studio ------------------ #
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_DEBUG "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(Symptogen PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif()
