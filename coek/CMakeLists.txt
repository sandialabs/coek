
##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    expr/ast_term.cpp
    expr/visitor_to_list.cpp
    expr/visitor_write_expr.cpp
    expr/visitor_to_MutableNLPExpr.cpp
    expr/visitor_to_QuadraticExpr.cpp
    expr/visitor_symdiff.cpp
    expr/visitor_mutable_values.cpp
    expr/visitor_variables.cpp
    expr/visitor_exprtemplate.cpp
    expr/varray.cpp
    expr/coek_expr.cpp
    abstract/expr_rule.cpp
    set/ast_set.cpp
    set/coek_sets.cpp
    set/coek_indexed.cpp
    model/coek_model.cpp
    model/writer_lp.cpp
    model/writer_nl.cpp
    solvers/solver.cpp
    solvers/testsolver.cpp
    autograd/autograd.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

#
# EXPRMODEL OPTIONS
#

#MESSAGE("-- Building Source with ExprModel: Default")
#list(APPEND sources exprmodel/exprmodel_default.cpp)

#
# ADMODEL OPTIONS
#

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Source with AD:        CppAd")
  list(APPEND sources
    autograd/cppad_repn.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
  list(APPEND autograd_flags "-DWITH_CPPAD")
endif()

# ASL LIBRARY
#if(NOT with_cppad AND with_asl)
#  MESSAGE("-- Building Source with AD:        ASL")
#  list(APPEND sources
#	autograd/asl_model.cpp)
#endif()

# Default AD LIBRARY
#if(NOT with_cppad AND NOT with_asl)
#  MESSAGE("-- Building Source with AD:        Simple")
#  list(APPEND sources
#    admodel/simplead.cpp)
#  add_definitions(-DWITH_ADMODEL_SIMPLEAD)
#endif()

#
# SOLVER OPTIONS
#

# GUROBI LIBRARY
if(with_gurobi)
    list(APPEND sources
        solvers/gurobi.cpp)
    if(NOT "$ENV{GUROBI_HOME}" STREQUAL "")
        MESSAGE("-- GUROBI_HOME $ENV{GUROBI_HOME}")
        INCLUDE_DIRECTORIES($ENV{GUROBI_HOME}/include)
    else()
        MESSAGE("-- GUROBI_HOME Not provided")
    endif()
endif()

# IPOPT LIBRARY
if(with_ipopt)
    list(APPEND sources
        solvers/ipopt.cpp)
    if(NOT "$ENV{IPOPT_HOME}" STREQUAL "")
        MESSAGE("-- IPOPT_HOME=$ENV{IPOPT_HOME}")
        INCLUDE_DIRECTORIES(PUBLIC $ENV{IPOPT_HOME}/include/coin)
    else()
        MESSAGE("-- IPOPT_HOME Not provided")
    endif()
endif()

#
# Solver Interface Options
#
if(with_cffi)
    list(APPEND sources
        capi/coek_capi.cpp)
        INCLUDE_DIRECTORIES(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coek/capi)
endif()

# build library
ADD_LIBRARY(coek SHARED ${sources})
TARGET_COMPILE_OPTIONS(coek PRIVATE ${solver_flags})
TARGET_COMPILE_OPTIONS(coek PRIVATE ${autograd_flags})
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(coek PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(coek PUBLIC gcov)
endif()


# make install
INSTALL(TARGETS coek
        DESTINATION lib
       )
INSTALL(FILES coek_model.hpp coek_expr.hpp coek_sets.hpp coek_indexed.hpp
        DESTINATION include
       )

