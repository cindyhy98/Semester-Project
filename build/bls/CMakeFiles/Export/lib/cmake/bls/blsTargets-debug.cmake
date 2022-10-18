#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bls::bls256" for configuration "Debug"
set_property(TARGET bls::bls256 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(bls::bls256 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libbls256.1.10.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libbls256.1.10.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS bls::bls256 )
list(APPEND _IMPORT_CHECK_FILES_FOR_bls::bls256 "${_IMPORT_PREFIX}/lib/libbls256.1.10.dylib" )

# Import target "bls::bls384" for configuration "Debug"
set_property(TARGET bls::bls384 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(bls::bls384 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libbls384.1.10.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libbls384.1.10.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS bls::bls384 )
list(APPEND _IMPORT_CHECK_FILES_FOR_bls::bls384 "${_IMPORT_PREFIX}/lib/libbls384.1.10.dylib" )

# Import target "bls::bls384_256" for configuration "Debug"
set_property(TARGET bls::bls384_256 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(bls::bls384_256 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libbls384_256.1.10.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libbls384_256.1.10.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS bls::bls384_256 )
list(APPEND _IMPORT_CHECK_FILES_FOR_bls::bls384_256 "${_IMPORT_PREFIX}/lib/libbls384_256.1.10.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
