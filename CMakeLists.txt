cmake_minimum_required(VERSION 2.8.12)
PROJECT(coek_testmodels
        )
if(${CMAKE_VERSION} VERSION_LESS "3.5.0")
    cmake_policy(SET CMP0022 NEW)
endif()

#
# GLOBAL INCLUDES
#
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

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

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})


##################### Options #####################

set(coek_dir "" CACHE FILEPATH "Set the COEK installation directory")

# Build options
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_caliper "Use the Caliper library" OFF)

# Solver Dependencies
option(with_gurobi "Use the Gurobi solver" OFF)
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")

message("Configuration Options")
message("-- coek_dir: ${coek_dir}")
message("-- with_debug: ${with_debug}")
message("-- with_verbose: ${with_verbose}")
message("-- with_gcov: ${with_gcov}")
message("-- with_gprof: ${with_gprof}")
message("-- with_caliper: ${with_caliper}")
message("-- with_gurobi: ${with_gurobi}")


##################### COEK configuration #####################

if("${coek_dir}" STREQUAL "")
else()
    if(NOT IS_ABSOLUTE ${coek_dir})
        set(coek_dir "${CMAKE_CURRENT_BINARY_DIR}/${coek_dir}")
    endif()
    list(APPEND CMAKE_PREFIX_PATH "${coek_dir}")
endif()
message("")
message("COEK Installation Configuration")
message("-- COEK_DIR:  ${coek_dir}")
message("-- COEK_HOME: $ENV{COEK_HOME}")
find_package(coek REQUIRED)
message("")
message("-- CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")


##################### Checks for compiler #####################

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


if(${with_gcov})
  set(CMAKE_BUILD_TYPE debug)
else()
  set(CMAKE_BUILD_TYPE release)
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
if (!MSVC)
    if(${with_gprof})
        set(CMAKE_CXX_FLAGS_RELEASE "-pg")
        MESSAGE("-- With gprof: on")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
        MESSAGE("-- With gprof: off")
    endif()
endif()

##################### SubDirectories #####################
# coek
add_subdirectory(coek)

if(with_gurobi)
    find_package(GUROBI)
    add_subdirectory(gurobi)
    list(APPEND solver_flags "-DWITH_GUROBI")
    MESSAGE("-- With Gurobi Solver: on")
else()
    MESSAGE("-- With Gurobi Solver: off")
endif()


