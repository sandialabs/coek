
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../catch/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

enable_testing()

SET(sources
    runner.cpp
    test_expr.cpp
   )

# testing binary
add_executable(runner ${sources})
target_link_libraries(runner Catch)
target_link_libraries(runner coek)

add_test(
  NAME runner
  COMMAND $<TARGET_FILE:runner> --success
)

