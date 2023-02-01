cmake_minimum_required(VERSION 3.12)
PROJECT(demos)

#
# For now, let's just assume we have C++17
#
#set(CMAKE_CXX_STANDARD 17)

MESSAGE("-- With Gurobi Solver: ${with_gurobi}")
if(with_gurobi)
    find_package(GUROBI)
    list(APPEND solver_flags "-DWITH_GUROBI")
endif()


include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# demo1
add_executable(demo1 demo1.cpp)
TARGET_LINK_LIBRARIES(demo1 PRIVATE coek::coek)
if (${with_gprof})
    target_compile_options(demo1 PUBLIC "-pg")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

