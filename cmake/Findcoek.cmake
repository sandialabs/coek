find_path(
  COEK_INCLUDE_DIRS
  NAMES coek/coek.hpp
  HINTS ${coek_dir} $ENV{COEK_HOME}
  PATH_SUFFIXES include)

find_library(
  COEK_LIBRARY
  NAMES coek
  HINTS ${coek_dir} $ENV{COEK_HOME}
  PATH_SUFFIXES lib)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(coek DEFAULT_MSG
                                  COEK_LIBRARY
                                  COEK_INCLUDE_DIRS)

