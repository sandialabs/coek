
find_package(Catch2 REQUIRED)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_INSTALL_PREFIX}/include)

if (with_fmtlib)
    find_package(fmt REQUIRED)
endif()


SET(sources
    runner.cpp
    test_model.cpp
    test_expr_visitors.cpp
    test_testsolver.cpp
    test_examples.cpp
    test_writers.cpp
    test_readers.cpp
    test_sets.cpp
    test_indexed.cpp
    test_var.cpp
    test_param.cpp
    test_expr.cpp
    test_obj.cpp
    test_con.cpp
    test_autograd_unknown.cpp
    test_sequence.cpp
   )

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Test with AD:        CppAd")
  list(APPEND sources
    test_autograd_cppad.cpp)
endif()

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(BEFORE ${coek_include_directories})
LINK_DIRECTORIES(${coek_link_directories})


enable_testing()


# runner
add_executable(runner ${sources})
TARGET_LINK_LIBRARIES(runner PUBLIC coek coekexamples Catch2::Catch2 ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(runner PUBLIC ${coek_compile_options})
target_compile_definitions(runner PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

include(CTest)
include(Catch)
catch_discover_tests(runner)

# demo1
add_executable(demo1 demo1.cpp)
TARGET_LINK_LIBRARIES(demo1 PUBLIC coek ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(demo1 PUBLIC ${coek_compile_options})
target_compile_definitions(demo1 PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

