
cmake_minimum_required(VERSION 3.7)

PROJECT( Coek )

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/Catch2/contrib;${CMAKE_MODULE_PATH})


##################### Options #####################

# Configuration options
option(with_tests "Build tests" OFF)
option(with_debug "Debug build" OFF)
option(with_gcov "Add code coverage using gcov" OFF)

# Autograd Dependencies
option(with_cppad "Use the CppAD Autograd Library" OFF)
option(with_sacado "Use the Sacado Autograd Library" OFF)
option(with_asl "Use the ASL Autograd Library" OFF)


# Solver Dependencies
option(with_ipopt "Use the Ipopt Solver" OFF)
option(with_gurobi "Use the Gurobi Solver" OFF)
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")


##################### Build Configurations #####################

# define build type
if(${with_gcov})
  set(CMAKE_BUILD_TYPE debug)
else()
  set(CMAKE_BUILD_TYPE release)
endif()
if(${with_debug})
  set(CMAKE_BUILD_TYPE debug)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

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


##################### Checks for compiler #####################

# TODO: Verify these minimum requirements

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
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



##################### SubDirectories #####################

# third_party
add_subdirectory(third_party)

# src
add_subdirectory(src)

# testing
if(with_tests)
  MESSAGE("-- With Tests: on")

  add_subdirectory(tests)
  include(CTest)

  add_custom_target(test_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
  add_custom_target(test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log")

  if (gcovr_FOUND)
    add_custom_target(gcovr 
      COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_CURRENT_SOURCE_DIR} -e ${CMAKE_CURRENT_SOURCE_DIR}/third_party -e ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  endif()
else()
  MESSAGE("-- With Tests: off")
endif()

