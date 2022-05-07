
find_package(Catch2 REQUIRED)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_INSTALL_PREFIX}/include)

if (fmtlib_available)
    find_package(fmt REQUIRED)
endif()


SET(sources
    runner.cpp
    test_expr.cpp
    test_model.cpp
    test_expr_visitors.cpp
    test_testsolver.cpp
    test_examples.cpp
    test_writers.cpp
    test_readers.cpp
    test_sets.cpp
    test_indexed.cpp
    test_autograd_unknown.cpp
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

message("LINK LIBS: ${coek_link_libraries}")

# demo1
add_executable(demo1 demo1.cpp)
TARGET_LINK_LIBRARIES(demo1 PUBLIC coek ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(demo1 PUBLIC ${coek_compile_options})

# lqcp
add_executable(lqcp lqcp.cpp)
TARGET_LINK_LIBRARIES(lqcp PUBLIC coek coekexamples ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(lqcp PUBLIC ${coek_compile_options})

# lqcp_array
if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    message("OPTIONS ${coek_compile_options}")
    add_executable(lqcp_array lqcp_array.cpp)
    TARGET_LINK_LIBRARIES(lqcp_array PUBLIC coek coekexamples ${coek_link_libraries})
    TARGET_COMPILE_OPTIONS(lqcp_array PUBLIC ${coek_compile_options})
endif()

# lqcp_map
if (with_compact)
    add_executable(lqcp_map lqcp_map.cpp)
    TARGET_LINK_LIBRARIES(lqcp_map PUBLIC coek coekexamples ${coek_link_libraries})
    TARGET_COMPILE_OPTIONS(lqcp_map PUBLIC ${coek_compile_options})
endif()

# lqcp2
add_executable(lqcp2 lqcp2.cpp)
TARGET_LINK_LIBRARIES(lqcp2 PUBLIC coek coekexamples ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(lqcp2 PUBLIC ${coek_compile_options})

# rlqcp
add_executable(rlqcp rlqcp.cpp)
TARGET_LINK_LIBRARIES(rlqcp PUBLIC coek coekexamples ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(rlqcp PUBLIC ${coek_compile_options})

# resolve
add_executable(resolve resolve.cpp)
TARGET_LINK_LIBRARIES(resolve PUBLIC coek coekexamples ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(resolve PUBLIC ${coek_compile_options})
target_compile_definitions(demo1 PUBLIC COEK_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

