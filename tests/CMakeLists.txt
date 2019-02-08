
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Catch2/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

SET(sources
    runner.cpp
    test_expr.cpp
    test_expr_manager_objects.cpp
    test_capi.cpp
    test_simple_model.cpp
   )

#
# Setup include directories and customize the sources built
#
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
if(with_cppad)
  MESSAGE("-- Testing Source with CppAd Library: on")
  list(APPEND sources
    test_cppad_model.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
else()
  MESSAGE("-- Testing Source with CppAd Library: off")
endif()


enable_testing()

#
# Create a testing binary for Catch2 tests
#
add_executable(runner ${sources})
target_link_libraries(runner Catch)
target_link_libraries(runner coek)
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(runner gcov)
endif()
if(${with_cppad})
  TARGET_COMPILE_OPTIONS(runner PUBLIC -DWITH_CPPAD)
endif()
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(runner ipopt)
  LINK_DIRECTORIES(${IPOPT_HOME}/lib)
endif()

include(CTest)
include(Catch)
catch_discover_tests(runner)

