
cmake_minimum_required(VERSION 3.10)

PROJECT( Coek )

# define build type
set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")


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


##################### Configure Python #####################

#look for python
#find_package(PythonLibs REQUIRED)
#include_directories(${PYTHON_INCLUDE_PATH})
#MESSAGE("-- PYTHON_INCLUDE_PATH ${PYTHON_INCLUDE_PATH}")
#MESSAGE("-- PYTHON_LIBRARIES ${PYTHON_LIBRARIES}")


##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(COEK_SRC
    expr_api.cpp
    expr_api.h
    expr_types.cpp
    intrinsic.cpp
    expr_intrinsic.h
    model.cpp
   )

# build library
ADD_LIBRARY(coek SHARED ${COEK_SRC})
if($GUROBI_HOME)
    MESSAGE("-- GUROBI_HOME ${GUROBI_HOME}")
    TARGET_INCLUDE_DIRECTORIES(coek ${GUROBI_HOME}/include)
else()
    MESSAGE("-- GUROBI_HOME Not provided")
endif()
TARGET_LINK_LIBRARIES(coek ${PYTHON_LIBRARIES})

