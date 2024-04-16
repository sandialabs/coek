cmake_minimum_required(VERSION 3.12)
project(coek_tpl)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})


##################### Options #####################

# Download options
option(insecure_downloads "If enabled, then downloads third party software without verifying the SSL connection" OFF)

# Build options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Install options
option(install_asl "Install the ASL library" OFF)
option(install_catch2 "Install the Catch2 library" OFF)
option(install_cppad "Install the CppAD autograd library" OFF)
option(install_fmtlib "Install the FMT library" OFF)
option(install_pybind11 "Install the Pybind11 library" OFF)
option(install_rapidjson "Install the RapidJson library" OFF)
option(install_all_tpls "Install the all third-party libraries" ON)
if (install_all_tpls)
    set(install_asl ON)
    set(install_catch2 ON)
    set(install_cppad ON)
    set(install_fmtlib ON)
    set(install_pybind11 ON)
    set(install_rapidjson ON)
endif()
option(use_superbuild "Use superbuild logic to install dependencies" ON)
set(install_dependencies OFF)
if (use_superbuild)
    if (install_catch2 OR install_cppad OR install_fmtlib OR install_pybind11 OR install_rapidjson OR install_asl)
        set(build_dependencies ON)
    endif()
endif()
if (install_catch2 OR install_cppad OR install_fmtlib OR install_pybind11 OR install_rapidjson OR install_asl)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${CMAKE_INSTALL_PREFIX}/lib64")
endif()

# Configuration options
#option(with_asl "Use the ASL autograd library" OFF)
#option(with_catch2 "Add tests using Catch2" OFF)
#option(with_cppad "Use the CppAD autograd library" OFF)
#option(with_cppyy "Use the cppyy library" OFF)
#option(with_fmtlib "Use the fmtlib library" OFF)
#option(with_pybind11 "Use the pybind11 library" OFF)
#option(with_rapidjson "Add support for reading JSON/JPOF files" OFF)


include(download_dir)
include(BuildThirdParty)
include(thirdparty.cmake)

setup_builds()
if (build_dependencies)
    return()
endif()

if (with_asl AND (NOT install_asl))
    find_package(ASL)
    if (ASL_FOUND)
    else()
        message("ERROR - ASL was not installed")
        set(with_asl OFF)
    endif()
endif()

if (with_catch2 AND (NOT install_catch2))
    find_package(Catch2)
    if (catch2_FOUND)
    else()
        message("ERROR - Catch2 was not installed")
        set(with_catch2 OFF)
    endif()
endif()

if (with_cppad AND (NOT install_cppad))
    find_package(cppad)
    if (cppad_FOUND)
    else()
        message("ERROR - CppAD was not installed")
        set(with_cppad OFF)
    endif()
endif()

if (with_rapidjson AND (NOT install_rapidjson))
    find_package(RapidJSON)
    if (RapidJSON_FOUND)
    else()
        message("ERROR - RapidJSON was not installed")
        set(with_rapidjson OFF)
    endif()
endif()

message("TPL SUMMARY")
message("-- ASL:       ${with_asl}")
message("-- Catch2:    ${with_catch2}")
message("-- CppAD:     ${with_cppad}")
message("-- RapidJSON: ${with_rapidjson}")

