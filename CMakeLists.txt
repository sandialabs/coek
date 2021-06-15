cmake_minimum_required(VERSION 3.12)
PROJECT(Coek
        LANGUAGES CXX
        VERSION 0.1
        DESCRIPTION "C++ Optimization Expression Kernels"
        )


if(DEFINED ENV{CONDA_PREFIX})
    message(STATUS "Building in a conda environment.")
    set(CONDA_ACTIVE TRUE)
    set(CMAKE_INSTALL_PREFIX "$ENV{CONDA_PREFIX}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}")
    set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")
    #include_directories($ENV{CONDA_PREFIX}/include)
    set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/pybind11/tools;${CMAKE_MODULE_PATH})


##################### Options #####################

# Configuration options
option(with_compact "Add support for compact expressions" OFF)
option(with_tests "Build tests" OFF)
option(with_examples "Build examples" OFF)
option(with_debug "Debug build" OFF)
option(with_verbose "Verbose build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)
option(with_gprof "Profile build" OFF)
option(with_caliper "Use the Caliper library" OFF)
option(with_fmtlib "Use the fmtlib library" OFF)
option(with_pybind11 "Build python interface using pybind11" OFF)
option(with_cppyy "Build python interface using cppyy" OFF)
option(with_docs "Build documentation" OFF)

# Autograd Dependencies
option(with_cppad "Use the CppAD autograd library" OFF)
option(with_sacado "Use the Sacado autograd library" OFF)
option(with_asl "Use the ASL autograd library" OFF)

# Solver Dependencies
option(with_ipopt "Use the Ipopt solver" OFF)
#set(IPOPT_HOME "" CACHE FILEPATH "Set the path to ipopt")
option(with_gurobi "Use the Gurobi solver" OFF)
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")

message("Configuration Options")

##################### Checks for compiler #####################

include(CheckCXXCompilerFlag)
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
    MESSAGE("-- With gcov code coverage: on")
  else()
    MESSAGE("-- With gcov code coverage: off")
    set(with_gcov OFF)
  endif()
else()
  MESSAGE("-- With gcov code coverage: off")
endif()
if (with_tests)
  find_package(gcovr)
  if (gcovr_FOUND)
    MESSAGE("-- With gcovr code coverage: on")
  else()
    MESSAGE("-- With gcovr code coverage: off")
  endif()
endif()

if(with_debug)
  MESSAGE("-- With Debug: on")
else()
  MESSAGE("-- With Debug: off")
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


##################### Other Config #####################

# define build type
if(with_compact)
    MESSAGE("-- With Compact Models: on")
    list(APPEND coek_compile_options "-DCOEK_WITH_COMPACT_MODEL")
else()
    MESSAGE("-- With Compact Models: off")
endif()


##################### Third Party Packages #############

add_subdirectory(third_party)
#
# Summary after looking for third-party packages
#
MESSAGE("-- Third Party Package Dependencies")
MESSAGE("   RapidJSON Library: ${rapidjson_available}")
MESSAGE("   Catch2 Library:    ${catch2_available}")
MESSAGE("   CppAD Library:     ${cppad_available}")
if (catch2_available)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/Catch2/contrib;${CMAKE_MODULE_PATH})
else()
    set(with_tests OFF)
endif()

MESSAGE("-- Third Party Include Directories")
if (rapidjson_available)
    MESSAGE("   RapidJSON: ${RAPIDJSON_INCLUDE_DIR}")
endif()
if (catch2_available)
    MESSAGE("   Catch2: ${CATCH2_INCLUDE_DIR}")
endif()
if (cppad_available)
    MESSAGE("   CppAD: ${CPPAD_INCLUDE_DIR}")
endif()


##################### Solvers #####################

# solvers
if(with_ipopt)
  list(APPEND solver_flags "-DWITH_IPOPT")
  MESSAGE("-- With Ipopt Solver: on")
else()
  MESSAGE("-- With Ipopt Solver: off")
endif()
if(with_gurobi)
  list(APPEND solver_flags "-DWITH_GUROBI")
  MESSAGE("-- With Gurobi Solver: on")
else()
  MESSAGE("-- With Gurobi Solver: off")
endif()

# docs
if (with_docs)
    MESSAGE("-- With Docs: on")
else()
    MESSAGE("-- With Docs: off")
endif()


##################### SubDirectories #####################

# coek
add_subdirectory(coek)

# examples
if (with_examples OR with_tests)
    add_subdirectory(examples)
endif()

# testing
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
      COMMAND ${GCOVR_EXECUTABLE} -d -r ${CMAKE_CURRENT_SOURCE_DIR} -e ../third_party/ -e ../test/)
  endif()
else()
  MESSAGE("-- With Tests: off")
endif()

#if(with_pybind11 OR with_cppyy OR with_cffi)
#    if((with_cffi AND with_cppyy) OR (with_cffi AND with_pybind11) OR (with_cppyy AND with_pybind11))
#        MESSAGE(FATAL_ERROR "Cannot configure multiple python interfaces!")
#    endif()
#endif()

# pybind11
if(with_pybind11)
    MESSAGE("-- With Pybind11: on")
    add_subdirectory(third_party/pybind11)
    add_subdirectory(pycoek/pybind11)

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
