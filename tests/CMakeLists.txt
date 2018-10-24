
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../catch/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

enable_testing()

SET(sources
    runner.cpp
    test_expr.cpp
   )

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

#
# Execute the Catch2 tests using CTest
#
add_test(
  NAME runner
  COMMAND $<TARGET_FILE:runner> -d yes
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

