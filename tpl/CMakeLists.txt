cmake_minimum_required(VERSION 3.12)
project(coek_tpl)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
#set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})


##################### Options #####################

# Download options
option(insecure_downloads "If enabled, then downloads third party software without verifying the SSL connection" OFF)

# Build options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Install options
option(install_catch2 "Install Catch2" OFF)
option(install_cppad "Install the CppAD autograd library" OFF)
option(install_fmtlib "Install the FMT library" OFF)
option(install_pybind11 "Install the Pybind11 library" OFF)
option(install_rapidjson "Install the RapidJson library" OFF)
option(install_all_tpls "Install the all third-party libraries" ON)
if (install_all_tpls)
    set(install_catch2 ON)
    set(install_cppad ON)
    set(install_fmtlib ON)
    set(install_pybind11 ON)
    set(install_rapidjson ON)
endif()
option(use_superbuild "Use superbuild logic to install dependencies" ON)
set(install_dependencies OFF)
if (use_superbuild)
    if (install_catch2 OR install_cppad OR install_fmtlib OR install_pybind11 OR install_rapidjson)
        set(build_dependencies ON)
    endif()
endif()
if (install_catch2 OR install_cppad OR install_fmtlib OR install_pybind11 OR install_rapidjson)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${CMAKE_INSTALL_PREFIX}/lib64")
endif()

# Configuration options
option(with_catch2 "Add tests using Catch2" ${install_catch2})
option(with_fmtlib "Use the fmtlib library" ${install_fmtlib})
option(with_rapidjson "Add support for reading JSON/JPOF files" ${install_rapidjson})
option(with_cppad "Use the CppAD autograd library" ${build_cppad})


include(download_dir)
include(BuildThirdParty)
include(thirdparty.cmake)

setup_builds()
if (build_dependencies)
    return()
endif()

if (with_catch2 AND (NOT install_catch2))
    find_package(Catch2)
    if (catch2_FOUND)
        set(catch2_available ON)
    else()
        set(catch2_available OFF)
    endif()
endif()

if (with_cppad AND (NOT install_cppad))
    find_package(cppad)
    if (cppad_FOUND)
        set(cppad_available ON)
    else()
        set(cppad_available OFF)
    endif()
endif()

if (with_rapidjson AND (NOT install_rapidjson))
    find_package(RapidJSON)
    if (RapidJSON_FOUND)
        set(rapidjson_available ON)
    else()
        set(rapidjson_available OFF)
    endif()
endif()

message("TPL SUMMARY")
message("-- Catch2 available:    ${catch2_available}")
message("-- CppAD available:     ${cppad_available}")
message("-- RapidJSON available: ${rapidjson_available}")

