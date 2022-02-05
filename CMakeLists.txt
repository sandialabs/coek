cmake_minimum_required(VERSION 2.8.12)
PROJECT(coek
        #LANGUAGES CXX
        #VERSION 0.1
        #DESCRIPTION "C++ Optimization Expression Kernels"
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

option(conda_install "If enabled, then install in the current conda environment" OFF)

if( (conda_install) AND (DEFINED ENV{CONDA_PREFIX}) AND (with_pybind11 OR with_cppyy) )
    message(STATUS "Installing in a conda environment: $ENV{CONDA_PREFIX}")
    set(CONDA_ACTIVE TRUE)
    set(CMAKE_INSTALL_PREFIX "$ENV{CONDA_PREFIX}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}")
    set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")
    #include_directories($ENV{CONDA_PREFIX}/include)
    set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})



##################### Options #####################

# Download options
option(insecure_downloads "If enabled, then builds with download third party software without verifying the SSL connection" OFF)

# Build options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(build_catch2 "Build Catch2" OFF)
option(build_cppad "Build the CppAD autograd library" OFF)
option(build_fmtlib "Build the FMT library" OFF)
option(build_pybind11 "Build the Pybind11 library" OFF)
option(build_rapidjson "Build the RapidJson library" OFF)
option(build_all "Build the all third-party libraries" OFF)
if (build_all)
    set(build_catch2 ON)
    set(build_cppad ON)
    set(build_fmtlib ON)
    set(build_pybind11 ON)
    set(build_rapidjson ON)
endif()
option(use_superbuild "Use superbuild logic to build dependencies" ON)
set(build_dependencies OFF)
if (use_superbuild)
    if (build_catch2 OR build_cppad OR build_fmtlib OR build_pybind11 OR build_rapidjson)
        set(build_dependencies ON)
    endif()
endif()
if (build_catch2 OR build_cppad OR build_fmtlib OR build_pybind11 OR build_rapidjson)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${CMAKE_INSTALL_PREFIX}/lib64")
endif()

# Configuration options
option(with_catch2 "Add tests using Catch2" ${build_catch2})
option(with_fmtlib "Use the fmtlib library" ${build_fmtlib})
option(with_pybind11 "Build python interface using pybind11" OFF)
option(with_rapidjson "Add support for reading JSON/JPOF files" ${build_rapidjson})

option(with_compact "Add support for compact expressions" OFF)
option(with_tests "Build tests" OFF)
option(with_examples "Build examples" OFF)
option(with_debug "Debug build" OFF)
option(build_shared_libs "Build a shared library" ON)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_caliper "Use the Caliper library" OFF)
option(with_cppyy "Build python interface using cppyy" OFF)
option(with_docs "Build documentation" OFF)

# Autograd Dependencies
option(with_cppad "Use the CppAD autograd library" ${build_cppad})
option(with_sacado "Use the Sacado autograd library" OFF)
option(with_asl "Use the ASL autograd library" OFF)

# Solver Dependencies
option(with_gurobi "Use the Gurobi solver" OFF)
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")

message("Configuration Options")


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
else()
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_STANDARD 11)
    MESSAGE("-- C++ version: C++11")
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

message("-- With Debug: ${with_debug}")
message("-- With Staticlib: ${with_staticlib}")

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


##################### Other Config #####################

# define build type
if(with_compact)
    MESSAGE("-- With Compact Models: on")
    list(APPEND coek_compile_options "-DCOEK_WITH_COMPACT_MODEL")
else()
    MESSAGE("-- With Compact Models: off")
endif()

##################### Solvers #####################

if(with_gurobi)
  list(APPEND solver_flags "-DWITH_GUROBI")
  MESSAGE("-- With Gurobi Solver: on")
else()
  MESSAGE("-- With Gurobi Solver: off")
endif()

##################### Docs #####################

if (with_docs)
    MESSAGE("-- With Docs: on")
else()
    MESSAGE("-- With Docs: off")
endif()



##################### Third Party Packages #############

include(download_dir)
include(BuildThirdParty)
include(thirdparty.cmake)

setup_builds()
if (build_dependencies)
    return()
endif()

if (with_catch2 AND (NOT build_catch2))
    find_package(Catch2)
    if (catch2_FOUND)
        set(catch2_available ON)
    else()
        set(catch2_available OFF)
    endif()
endif()

if (with_cppad AND (NOT build_cppad))
    find_package(cppad)
    if (cppad_FOUND)
        set(cppad_available ON)
    else()
        set(cppad_available OFF)
    endif()
endif()

if (with_rapidjson AND (NOT build_rapidjson))
    find_package(RapidJSON)
    if (RapidJSON_FOUND)
        set(rapidjson_available ON)
    else()
        set(rapidjson_available OFF)
    endif()
endif()

MESSAGE("-- Catch2 available:    ${catch2_available}")
MESSAGE("-- CppAD available:     ${cppad_available}")
MESSAGE("-- RapidJSON available: ${rapidjson_available}")

##################### SubDirectories #####################
# coek
add_subdirectory(coek)

# examples
if (with_examples OR with_tests)
    add_subdirectory(examples)
endif()

# testing
if (NOT catch2_available)
    set(with_tests OFF)
endif()
if (with_tests)
  MESSAGE("-- With Tests: on")

  add_subdirectory(test)
  include(CTest)

  add_custom_target(test_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
  add_custom_target(test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log")

  if (gcovr_FOUND)
    add_custom_target(gcovr 
      COMMAND ${GCOVR_EXECUTABLE} -d -r ${CMAKE_CURRENT_SOURCE_DIR} -e ../third_party/ -e ../test/ -e install/ )
  endif()
else()
  MESSAGE("-- With Tests: off")
endif()

# pybind11
if(with_pybind11)
    MESSAGE("-- With Pybind11: on")
    find_package(pybind11)
    add_subdirectory(pycoek/pybind11)
    if (build_pybind11)
        set(CMAKE_MODULE_PATH ${download_dir}/pybind11/tools;${CMAKE_MODULE_PATH})
    endif()

    #
    # Note that this target is automatically build when with_pybind11 is ON
    #
    add_custom_target(python_pybind11
        DEPENDS pycoek_pybind11)
    add_custom_target(test_python_pybind11
        DEPENDS pycoek_pybind11
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/pycoek/pybind11 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
else()
    MESSAGE("-- With Pybind11: off")
endif()

# cppyy
if(with_cppyy)
    MESSAGE("-- With Cppyy: on")
    add_subdirectory(pycoek/cppyy)

    #
    # Note that this target is automatically build when with_cppyy is ON
    #
    add_custom_target(python_cppyy
        DEPENDS pycoek_cppyy)
    add_custom_target(test_python_cppyy
        DEPENDS pycoek_cppyy
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/pycoek/cppyy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
else()
    MESSAGE("-- With Cppyy: off")
endif()

# docs
if (with_docs)
    add_subdirectory(doc)
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

install(EXPORT coekTargets
        FILE coekTargets.cmake
        NAMESPACE coek::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coek
)

