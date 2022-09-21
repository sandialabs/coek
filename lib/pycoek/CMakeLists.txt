cmake_minimum_required(VERSION 3.12)
PROJECT(pycoek)

#
# For now, let's just assume we have C++17
#
set(CMAKE_CXX_STANDARD 17)

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

option(conda_install "If enabled, then install in the current conda environment" OFF)

if( (conda_install) AND (DEFINED ENV{CONDA_PREFIX}) AND (with_pybind11 OR with_cppyy) )
    message(STATUS "Installing in a conda environment: $ENV{CONDA_PREFIX}")
    set(CONDA_ACTIVE TRUE)
    set(CMAKE_INSTALL_PREFIX "$ENV{CONDA_PREFIX}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}")
    set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")
    set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})


##################### Options #####################

# Build options
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_pybind11 "Build python interface using pybind11" OFF)
option(with_cppyy "Build python interface using cppyy" OFF)

message("Configuration Options")
message("-- coek_dir: ${coek_dir}")
message("-- with_debug: ${with_debug}")
message("-- with_verbose: ${with_verbose}")
message("-- with_gcov: ${with_gcov}")
message("-- with_gprof: ${with_gprof}")
message("-- with_pybind11: ${with_pybind11}")
message("-- with_cppyy: ${with_cppyy}")


##################### COEK configuration #####################

if("${coek_dir}" STREQUAL "")
    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install)
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install/lib/cmake/coek")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install/lib/cmake/fmt")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install/lib64/cmake/coek")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install/lib64/cmake/fmt")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../lib/coek/build/install/share/cmake/pybind11")
    endif()
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
message("-- CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
find_package(coek REQUIRED NO_MODULE)


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


##################### SubDirectories #####################

# pybind11
if(with_pybind11)
    MESSAGE("-- With Pybind11: on")
    find_package(pybind11)
    add_subdirectory(pybind11)
    #if (build_pybind11)
    #    set(CMAKE_MODULE_PATH ${download_dir}/pybind11/tools;${CMAKE_MODULE_PATH})
    #endif()

    #
    # Note that this target is automatically build when with_pybind11 is ON
    #
    add_custom_target(python_pybind11
        DEPENDS pycoek_pybind11)
    add_custom_target(test_python_pybind11
        DEPENDS pycoek_pybind11
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/pybind11 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
else()
    MESSAGE("-- With Pybind11: off")
endif()

# cppyy
if(with_cppyy)
    MESSAGE("-- With Cppyy: on")
    add_subdirectory(cppyy)

    #
    # Note that this target is automatically build when with_cppyy is ON
    #
    add_custom_target(python_cppyy
        DEPENDS pycoek_cppyy)
    add_custom_target(test_python_cppyy
        DEPENDS pycoek_cppyy
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/cppyy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
else()
    MESSAGE("-- With Cppyy: off")
endif()

# cffi
if(with_cffi)
    MESSAGE("-- With Cffi: on")
    add_subdirectory(cffi)

    #
    # Note that this target is automatically build when with_cffi is ON
    #
    add_custom_target(python_cffi
        DEPENDS pycoek)
    add_custom_target(test_python_cffi
        DEPENDS pycoek
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/cffi ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
else()
    MESSAGE("-- With Cffi: off")
endif()


# =============================================================================
# Provide config and version files to be used by other applications
# =============================================================================

#set(INCLUDE_INSTALL_DIR "include/" CACHE STRING "Where to install headers relative to prefix")
#set(LIBRARY_INSTALL_DIR "lib/" CACHE STRING "Where to install libraries relative to prefix")

