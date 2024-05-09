cmake_minimum_required(VERSION 3.13)
project(CoekProject)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})

#
# GLOBAL INCLUDES
#
find_package(Python COMPONENTS Interpreter Development)
include(Clang-Format)
include(Black-Format)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)
include(CMakePackageConfigHelpers)

#
# Change the default install directory for the sub-builds, as we will
# seldom have access to nor want to install to /usr/local. This can be
# overridden from the command line via cmake -DCMAKE_INSTALL_PREFIX=...
#
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install"
    CACHE PATH
    "Top level directory for installation"
    FORCE
  )
endif()

set(INCLUDE_INSTALL_DIR "include/" CACHE STRING "Where to install headers relative to prefix")
set(LIBRARY_INSTALL_DIR "lib/" CACHE STRING "Where to install libraries relative to prefix")

option(BUILD_SHARED_LIBS "Build a shared library" ON)

##################### TPL Configuration #####################
#
# Installation options for TPLs define default values of configuration options.
#
option(with_asl "Use the ASL autograd library" OFF)
option(with_catch2 "Add tests using Catch2" OFF)
option(with_cppad "Use the CppAD autograd library" OFF)
option(with_cppyy "Use the cppyy library" OFF)
option(with_fmtlib "Use the fmtlib library" OFF)
option(with_pybind11 "Use the pybind11 library" OFF)
option(with_rapidjson "Add support for reading JSON/JPOF files" OFF)

option(with_spack "Build with spack" OFF)
if (NOT ${with_spack})
message("")
add_subdirectory(tpl)

add_custom_target(install_tpls
        DEPENDS _install_tpls
        COMMAND ${CMAKE_COMMAND} -Dwith_pybind11=ON -Dwith_catch2=ON -Dwith_fmtlib=ON -Dwith_cppad=ON -Dwith_rapidjson=ON -Dwith_asl=ON ${PROJECT_SOURCE_DIR}
        )
add_custom_target(install_tpl
        DEPENDS _install_tpls
        COMMAND ${CMAKE_COMMAND} -Dwith_pybind11=ON -Dwith_catch2=ON -Dwith_fmtlib=ON -Dwith_cppad=ON -Dwith_rapidjson=ON -Dwith_asl=ON ${PROJECT_SOURCE_DIR}
        )
endif()

##################### Options #####################


# Build options
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_caliper "Use the Caliper library" OFF)
option(with_threads "Compiled with threads enabled" ON)
option(with_openmp "Compiled with openmp enabled" ON)
# Monorepo configuration
option(with_tests "Build tests" OFF)
option(with_apps "Build apps" ON)
option(with_docs "Build documentation" OFF)
option(with_python "Build/Install Python libs" OFF)

##################### Checks for compiler #####################

message("Revised Configuration Options")
if(with_compact)
    CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)

    if(COMPILER_SUPPORTS_CXX17)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    else()
      # TODO: Verify these minimum requirements
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
      if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.1)
          message(FATAL_ERROR "CLANG version must be at least 3.1!")
        endif()
      elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
          message(FATAL_ERROR "GCC version must be at least 4.7!")
        endif()
      elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12.0)
          message(FATAL_ERROR "ICC version must be at least 12.0!")
        endif()
      elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12.0)
           message(FATAL_ERROR "MSVC version must be at least 12.0!")
        endif()
      endif()
    endif()
    MESSAGE("-- C++ version: C++17")
elseif(NOT "${CMAKE_CXX_STANDARD}")
    CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    MESSAGE("-- C++ version: C++17")
else()
    MESSAGE("-- C++ version: C++${CMAKE_CXX_STANDARD}")
endif()

if (CMAKE_CXX_STANDARD LESS 17)
    message("DISABLING FMTLIB")
    set(with_fmtlib OFF)   
endif()

##################### General Configuration Options #####################

find_package(Threads)
if (NOT ${Threads_FOUND})
    set(with_threads OFF)
endif()
find_package(OpenMP)
if (NOT ${OpenMP_FOUND})
    set(with_openmp OFF)
endif()

message("Initial Configuration Options")
# Options defined here
message("-- with_debug: ${with_debug}")
message("-- with_verbose: ${with_verbose}")
message("-- with_gcov: ${with_gcov}")
message("-- with_gprof: ${with_gprof}")
message("-- with_caliper: ${with_caliper}")
message("-- with_tests: ${with_tests}")
message("-- with_docs: ${with_docs}")
message("-- with_python: ${with_python}")
message("-- with_threads: ${with_threads}")
message("-- with_openmp: ${with_openmp}")
# Options defined in tpl
message("-- with_asl: ${with_asl}")
message("-- with_catch2: ${with_catch2}")
message("-- with_cppad: ${with_cppad}")
message("-- with_cppyy: ${with_cppyy}")
message("-- with_fmtlib: ${with_fmtlib}")
message("-- with_pybind11: ${with_pybind11}")
message("-- with_rapidjson: ${with_rapidjson}")

##################### Other Configuraton Options #####################

if (with_gcov)
  if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
  else()
    set(with_gcov OFF)
  endif()
endif()
MESSAGE("-- With gcov: ${with_gcov}")
if (with_tests)
  find_package(gcovr)
  if (gcovr_FOUND)
    MESSAGE("-- With gcovr: ON")
  else()
    MESSAGE("-- With gcovr: OFF")
  endif()
endif()

message("-- With debug: ${with_debug}")
message("-- With gprof: ${with_gprof}")
message("-- With staticlib: ${with_staticlib}")

if(${with_gcov})
  set(CMAKE_BUILD_TYPE debug)
endif()

if(${with_verbose})
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
if(${with_debug})
    set(CMAKE_BUILD_TYPE debug)
    if (MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall /DDEBUG /permissive-")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG")
    endif()
else()
    if (MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "/permissive-")
    endif()
endif()

if(${with_tests})
    add_custom_target(test DEPENDS test_lib)
endif()

if(${with_docs})
    find_package(Sphinx REQUIRED)
    add_custom_target(docs DEPENDS doc_lib)
endif()


##################### Monorepo Subpackages #####################

message("")

message("Configuring Monorepo Packages")
add_subdirectory(lib)
if(${with_tests})
    add_subdirectory(test)
endif()
if(${with_apps})
    add_subdirectory(app)
endif()

message("")
