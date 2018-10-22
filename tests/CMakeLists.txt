
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../catch/single_include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

enable_testing()

# testing binary
add_executable(runner runner.cpp)
target_link_libraries(runner Catch)

add_test(
  NAME runner
  COMMAND $<TARGET_FILE:runner> --success
)

