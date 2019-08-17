
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Catch2/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

#SET(sources
#    test_expr.cpp
#    test_expr_manager_objects.cpp
#    test_capi.cpp
#   )

SET(sources
    runner.cpp
    test_model.cpp
    test_expr_visitors.cpp
    test_testsolver.cpp
    test_examples.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

#
# TESTING OPTIONS
#

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Test with AD:        CppAd")
  list(APPEND sources
    test_cppad_autograd.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
endif()


enable_testing()

#
# Create a testing binary for Catch2 tests
#
if(${with_ipopt})
  LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
endif()
if(${with_gurobi})
  LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
endif()

add_executable(runner ${sources})
TARGET_LINK_LIBRARIES(runner PUBLIC Catch)
TARGET_LINK_LIBRARIES(runner PUBLIC coek)
TARGET_LINK_LIBRARIES(runner PUBLIC coekexamples)
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(runner PUBLIC gcov)
endif()
if(${with_cppad})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -DWITH_CPPAD)
endif()
if(${with_ipopt})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -DWITH_IPOPT)
  TARGET_LINK_LIBRARIES(runner PUBLIC ipopt gfortran)
endif()
if(${with_gurobi})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -DWITH_GUROBI)
  TARGET_LINK_LIBRARIES(runner PUBLIC gurobi_g++5.2 gurobi81)
endif()

include(CTest)
include(Catch)
catch_discover_tests(runner)

# lqcp
add_executable(lqcp lqcp.cpp)
TARGET_LINK_LIBRARIES(lqcp PUBLIC coek)
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(lqcp PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(lqcp PUBLIC gcov)
endif()
if(${with_cppad})
  TARGET_COMPILE_OPTIONS(lqcp PUBLIC -DWITH_CPPAD)
endif()
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(lqcp PUBLIC ipopt gfortran)
endif()
if(${with_gurobi})
  TARGET_LINK_LIBRARIES(lqcp PUBLIC gurobi_g++5.2 gurobi81)
endif()

# rlqcp
add_executable(rlqcp rlqcp.cpp)
TARGET_LINK_LIBRARIES(rlqcp PUBLIC coek)
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(rlqcp PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(rlqcp PUBLIC gcov)
endif()
if(${with_cppad})
  TARGET_COMPILE_OPTIONS(rlqcp PUBLIC -DWITH_CPPAD)
endif()
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(rlqcp PUBLIC ipopt gfortran)
endif()
if(${with_gurobi})
  TARGET_LINK_LIBRARIES(rlqcp PUBLIC gurobi_g++5.2 gurobi81)
endif()


