#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "shaman::shaman" for configuration ""
set_property(TARGET shaman::shaman APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(shaman::shaman PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libshaman.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS shaman::shaman )
list(APPEND _IMPORT_CHECK_FILES_FOR_shaman::shaman "${_IMPORT_PREFIX}/lib64/libshaman.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
