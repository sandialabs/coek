
find_package(Catch2 REQUIRED)
add_library(Catch INTERFACE)

if (with_fmtlib)
    find_package(fmt REQUIRED)
endif()


SET(sources
    smoke/test_model.cpp
    smoke/test_visitor_simplify.cpp
    smoke/test_visitor_mutable.cpp
    smoke/test_visitor_writer.cpp
    smoke/test_visitor_quadexpr.cpp
    smoke/test_visitor_nlpexpr.cpp
    smoke/test_visitor_symdiff.cpp
    smoke/test_visitor_findvarparam.cpp
    smoke/test_visitor_eval.cpp
    smoke/test_testsolver.cpp
    smoke/test_writers.cpp
    smoke/test_readers.cpp
    smoke/test_sets.cpp
    smoke/test_indexed.cpp
    smoke/test_var.cpp
    smoke/test_param.cpp
    smoke/test_expr.cpp
    smoke/test_obj.cpp
    smoke/test_con.cpp
    smoke/test_subexpression.cpp
    smoke/test_autograd_unknown.cpp
    smoke/test_autograd_all.cpp
    smoke/test_util_sequence.cpp
    mt_funcs.cpp
    solver/test_examples.cpp
    solver/test_gurobi.cpp
    solver/test_highs.cpp
    solver/test_ipopt.cpp
    solver/TestModels.cpp
   )

if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    list(APPEND sources
        smoke/test_util_DataPortal.cpp
        )
endif()

set(coek_test_libraries)

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Test with AD:        CppAd")
  list(APPEND sources
    smoke/test_autograd_cppad.cpp)
endif()

# ASL LIBRARY
if(with_asl)
  MESSAGE("-- Building Test with AD:        ASL")
  list(APPEND sources
    smoke/test_autograd_asl.cpp)
endif()

# Thread tests
if (with_threads)
    list(APPEND coek_test_libraries Threads::Threads)
endif()

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(BEFORE ${coek_include_directories})
LINK_DIRECTORIES(${coek_link_directories})


enable_testing()


# runner
add_executable(runner ${sources})
TARGET_LINK_LIBRARIES(runner PUBLIC coek coekexamples Catch2::Catch2WithMain ${coek_link_libraries} ${coek_test_libraries})
TARGET_COMPILE_OPTIONS(runner PUBLIC ${coek_compile_options})
target_compile_definitions(runner PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

include(CTest)
include(Catch)
catch_discover_tests(runner)

# jpof_demo
add_executable(jpof_demo jpof_demo.cpp)
TARGET_LINK_LIBRARIES(jpof_demo PUBLIC coek ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(jpof_demo PUBLIC ${coek_compile_options})
target_compile_definitions(jpof_demo PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

# coek_micro_test
add_executable(coek_micro_test coek_micro_test.cpp)
TARGET_LINK_LIBRARIES(coek_micro_test PUBLIC coek ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(coek_micro_test PUBLIC ${coek_compile_options})
target_compile_definitions(coek_micro_test PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")


# testing
add_custom_target(test_coek
    COMMAND ${CMAKE_CTEST_COMMAND})
add_custom_target(test_coek_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_custom_target(test_coek_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log")

if (gcovr_FOUND)
add_custom_target(gcovr_coek
    COMMAND ${GCOVR_EXECUTABLE} -d -r ${coek_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..
)
add_custom_target(gcovr_coek_verbose
    COMMAND ${GCOVR_EXECUTABLE} -v -d -r ${coek_SOURCE_DIR} --filter "\.\." ${CMAKE_CURRENT_BINARY_DIR}/.. )
endif()

