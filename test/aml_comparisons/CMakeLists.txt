cmake_minimum_required(VERSION 3.12)
PROJECT(aml_comparisons)

#
# For now, let's just assume we have C++17
#
set(CMAKE_CXX_STANDARD 17)

##################### SubDirectories #####################
# coek
add_subdirectory(coek)

MESSAGE("-- With Gurobi Solver: ${with_gurobi}")
if(with_gurobi)
    find_package(GUROBI)
    add_subdirectory(gurobi)
    list(APPEND solver_flags "-DWITH_GUROBI")
endif()


