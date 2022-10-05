#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ReactPhysics3D::ReactPhysics3D" for configuration "MinSizeRel"
set_property(TARGET ReactPhysics3D::ReactPhysics3D APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(ReactPhysics3D::ReactPhysics3D PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/reactphysics3d.lib"
  )

list(APPEND _cmake_import_check_targets ReactPhysics3D::ReactPhysics3D )
list(APPEND _cmake_import_check_files_for_ReactPhysics3D::ReactPhysics3D "${_IMPORT_PREFIX}/lib/reactphysics3d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
