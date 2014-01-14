#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Box2D_shared" for configuration "Release"
set_property(TARGET Box2D_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Box2D_shared PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libBox2D.so.2.3.0"
  IMPORTED_SONAME_RELEASE "libBox2D.so.2.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS Box2D_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_Box2D_shared "${_IMPORT_PREFIX}/lib/libBox2D.so.2.3.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
