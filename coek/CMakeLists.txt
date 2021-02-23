
##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    ast/base_terms.cpp
    ast/constraint_terms.cpp
    ast/objective_terms.cpp
    ast/value_terms.cpp
    ast/expr_terms.cpp
    ast/visitor_to_list.cpp
    ast/visitor_write_expr.cpp
    ast/visitor_to_MutableNLPExpr.cpp
    ast/visitor_to_QuadraticExpr.cpp
    ast/visitor_symdiff.cpp
    ast/visitor_mutable_values.cpp
    ast/visitor_variables.cpp
    ast/varray.cpp
    api/constants.cpp
    api/expression.cpp
    api/expression_visitor.cpp
    api/objective.cpp
    api/constraint.cpp
    api/intrinsic_fn.cpp
    model/coek_model.cpp
    model/writer_lp.cpp
    model/writer_nl.cpp
    model/reader_jpof.cpp
    solvers/solver.cpp
    solvers/testsolver.cpp
    autograd/autograd.cpp
    abstract/expr_rule.cpp
   )

if(with_compact)
    list(APPEND sources
        ast/compact_terms.cpp
        compact/sequence_context.cpp
        compact/expression_sequence.cpp
        compact/objective_sequence.cpp
        compact/constraint_sequence.cpp
        compact/ast_set.cpp
        compact/coek_sets.cpp
        compact/coek_indexed.cpp
        compact/visitor_exprtemplate.cpp
        )
endif()

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Caliper LIBRARY
if(with_caliper)
    include_directories(BEFORE ${CALIPER_INCLUDE_DIR})
    list(APPEND coek_compile_options -DWITH_CALIPER)
    list(APPEND coek_link_libraries caliper)
    list(APPEND coek_link_directories ${CALIPER_LIBRARY_DIR})
    list(APPEND coek_include_directories ${CALIPER_INCLUDE_DIR})
endif()

# fmtlib LIBRARY
if(with_fmtlib)
    include_directories(BEFORE ${FMTLIB_INCLUDE_DIR})
    list(APPEND coek_compile_options -DWITH_FMTLIB -Wextra -Wconversion -Wpedantic -Wsign-conversion)
    list(APPEND coek_link_libraries fmt)
    list(APPEND coek_include_directories ${FMTLIB_INCLUDE_DIR})
endif()

if(with_rapidjson)
    include_directories(BEFORE ${RAPIDJSON_INCLUDE_DIR})
    list(APPEND coek_compile_options -DWITH_RAPIDJSON)
    list(APPEND coek_include_directories ${RAPIDJSON_INCLUDE_DIR})
endif()

# gcov
if(with_gcov)
    list(APPEND coek_compile_options -fprofile-arcs -ftest-coverage -g)
    list(APPEND coek_link_libraries gcov)
endif()

#
# ADMODEL OPTIONS
#

#
# CppAD LIBRARY
#
if(with_cppad)
    MESSAGE("-- Building Source with AD:        CppAd")
    list(APPEND sources
                autograd/cppad_repn.cpp)
    include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
    list(APPEND coek_compile_options "-DWITH_CPPAD")
    #list(APPEND coek_link_libraries cppad)
    list(APPEND coek_include_directories ${CPPAD_INCLUDE_DIR})
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

#
# GUROBI LIBRARY
#
if(with_gurobi)
    list(APPEND sources
                solvers/gurobi.cpp)
    if(NOT "$ENV{GUROBI_HOME}" STREQUAL "")
        MESSAGE("-- GUROBI_HOME $ENV{GUROBI_HOME}")
        INCLUDE_DIRECTORIES($ENV{GUROBI_HOME}/include)
    else()
        MESSAGE("-- GUROBI_HOME Not provided")
    endif()
    list(APPEND coek_compile_options -DWITH_GUROBI)
    list(APPEND coek_link_libraries gurobi_g++5.2 gurobi90)
    LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
endif()

#
# IPOPT LIBRARY
#
if(with_ipopt)
    list(APPEND sources
                solvers/ipopt.cpp)
    if(NOT "$ENV{IPOPT_HOME}" STREQUAL "")
        MESSAGE("-- IPOPT_HOME=$ENV{IPOPT_HOME}")
        INCLUDE_DIRECTORIES(PUBLIC $ENV{IPOPT_HOME}/include/coin)
    else()
        MESSAGE("-- IPOPT_HOME Not provided")
    endif()
    list(APPEND coek_compile_options -DWITH_IPOPT)
    list(APPEND coek_link_libraries ipopt gfortran)
    LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
endif()

#
# Solver Interface Options
#
if(with_cffi)
    list(APPEND sources
        capi/coek_capi.cpp)
        INCLUDE_DIRECTORIES(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coek/capi)
endif()

#
# Build library
#
ADD_LIBRARY(coek SHARED ${sources})
TARGET_COMPILE_OPTIONS(coek PRIVATE ${solver_flags})
TARGET_COMPILE_OPTIONS(coek PRIVATE ${coek_compile_options})
if(${with_gcov})
  TARGET_LINK_LIBRARIES(coek PUBLIC gcov)
endif()


#
# make install
#
INSTALL(TARGETS coek
        DESTINATION lib
       )
if(with_fmtlib)
    INSTALL(TARGETS fmt
        DESTINATION lib
       )
endif()
INSTALL(FILES
        coek.hpp 
        coek_model.hpp
        DESTINATION include/coek
       )
INSTALL(FILES
        api/expression.hpp
        api/expression_visitor.hpp
        api/intrinsic_fn.hpp
        api/objective.hpp
        api/constraint.hpp
        api/constants.hpp
        DESTINATION include/coek/api
       )
INSTALL(FILES
        compact/coek_sets.hpp
        compact/coek_indexed.hpp
        compact/sequence_context.hpp
        compact/expression_sequence.hpp
        compact/objective_sequence.hpp
        compact/constraint_sequence.hpp
        DESTINATION include/coek/compact
       )

#
# Push the compile options to the parent scope
#
set(coek_compile_options ${coek_compile_options} PARENT_SCOPE)
set(coek_link_libraries ${coek_link_libraries} PARENT_SCOPE)
set(coek_include_directories ${coek_include_directories} PARENT_SCOPE)
set(coek_link_directories ${coek_link_directories} PARENT_SCOPE)

