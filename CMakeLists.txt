cmake_minimum_required(VERSION 3.12)
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


##################### TPL Configuration #####################
#
# Installation options for TPLs define default values of configuration options.
#
message("")
add_subdirectory(tpl)

add_custom_target(install_tpls
        DEPENDS _install_tpls
        COMMAND ${CMAKE_COMMAND} -Dwith_pybind11=ON -Dwith_catch2=ON -Dwith_fmtlib=ON -Dwith_cppad=ON -Dwith_rapidjson=ON ${PROJECT_SOURCE_DIR}
        )
add_custom_target(install_tpl
        DEPENDS _install_tpls
        COMMAND ${CMAKE_COMMAND} -Dwith_pybind11=ON -Dwith_catch2=ON -Dwith_fmtlib=ON -Dwith_cppad=ON -Dwith_rapidjson=ON ${PROJECT_SOURCE_DIR}
        )

##################### Options #####################

option(build_shared_libs "Build a shared library" ON)

# Build options
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_caliper "Use the Caliper library" OFF)
# Monorepo configuration
option(with_tests "Build tests" OFF)
option(with_docs "Build documentation" OFF)
option(with_python "Build/Install Python libs" ${Python_FOUND})

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
# Options defined in tpl
message("-- with_catch2: ${with_catch2}")
message("-- with_cppad: ${with_cppad}")
message("-- with_pybind11: ${with_pybind11}")
message("-- with_cppyy: ${with_cppyy}")
message("-- with_rapidjson: ${with_rapidjson}")
message("-- with_fmtlib: ${with_fmtlib}")

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
add_subdirectory(test)
add_subdirectory(app)

message("")
