cmake_minimum_required(VERSION 3.12)
project(CoekProject)

##################### Options #####################

# Build options
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)

message("Configuration Options")
message("-- coek_dir: ${coek_dir}")
message("-- with_debug: ${with_debug}")
message("-- with_verbose: ${with_verbose}")
message("-- with_gcov: ${with_gcov}")
message("-- with_gprof: ${with_gprof}")

add_subdirectory(lib/coek)
add_subdirectory(lib/pycoek)
add_subdirectory(tpl)
#add_subdirectory(test/aml_comparisons)
