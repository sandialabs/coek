# This script defines a download_dir variable, if it's not already defined.

# Cache the downloads in download_dir if it's defined. Otherwise, use the
# user's typical Downloads directory, if it already exists. Otherwise, use a
# Downloads subdir in the build tree.
#
if(NOT DEFINED download_dir)
  if(NOT "$ENV{HOME}" STREQUAL "" AND EXISTS "$ENV{HOME}/downloads")
    set(download_dir "$ENV{HOME}/downloads")
  elseif(NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "")
    set(download_dir "${CMAKE_CURRENT_BINARY_DIR}/downloads")
  else()
    message(FATAL_ERROR "unexpectedly empty CMAKE_CURRENT_BINARY_DIR")
  endif()
  string(REPLACE "\\" "/" download_dir "${download_dir}")
endif()

file(MAKE_DIRECTORY "${download_dir}")
if(NOT EXISTS "${download_dir}")
  message(FATAL_ERROR "could not find or make 'downloads' directory")
endif()
