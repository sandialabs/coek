
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Catch2/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

SET(sources
    runner.cpp
    test_expr.cpp
    test_expr_manager_objects.cpp
    test_capi.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

#
# ADMODEL OPTIONS
#

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Test with AD:        CppAd")
  list(APPEND sources
    test_cppad_model.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
endif()

# ASL LIBRARY
if(NOT with_cppad AND with_asl)
  MESSAGE("-- Building Test with AD:        ASL")
  list(APPEND sources
    test_asl_model.cpp)
endif()

# Default AD LIBRARY
if(NOT with_cppad AND NOT with_asl)
  MESSAGE("-- Building Test with AD:        Simple")
  list(APPEND sources
    test_simple_model.cpp)
  add_definitions(-DWITH_ADMODEL_SIMPLEAD)
endif()


enable_testing()

#
# Create a testing binary for Catch2 tests
#
if(${with_ipopt})
  MESSAGE("HERE")
  LINK_DIRECTORIES(${IPOPT_HOME}/lib)
endif()

add_executable(runner ${sources})
target_link_libraries(runner PUBLIC Catch)
target_link_libraries(runner PUBLIC coek)
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(runner PUBLIC gcov)
endif()
if(${with_cppad})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -DWITH_CPPAD)
endif()
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(runner ipopt)
endif()

include(CTest)
include(Catch)
catch_discover_tests(runner)

