
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

# fmtlib LIBRARY
if(fmtlib_available)
    find_package(fmt REQUIRED)
    list(APPEND coek_compile_options -DWITH_FMTLIB -Wextra -Wconversion -Wpedantic -Wsign-conversion)
    list(APPEND coek_link_libraries fmt::fmt)
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include)
endif()

# Caliper LIBRARY
if(with_caliper)
    list(APPEND coek_compile_options -DWITH_CALIPER)
    list(APPEND coek_link_libraries caliper)
    list(APPEND coek_link_directories ${CALIPER_LIBRARY_DIR})
    list(APPEND coek_include_directories ${CALIPER_INCLUDE_DIR})
endif()

if(rapidjson_available)
    find_package(RapidJSON REQUIRED)
    list(APPEND coek_compile_options -DWITH_RAPIDJSON)
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include)
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
if(cppad_available)
    list(APPEND sources
                autograd/cppad_repn.cpp)
    list(APPEND coek_compile_options "-DWITH_CPPAD")
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include)
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
        list(APPEND coek_include_directories $ENV{GUROBI_HOME}/include)
        list(APPEND coek_link_directories $ENV{GUROBI_HOME}/lib)
    else()
        MESSAGE("-- GUROBI_HOME Not provided")
    endif()
    list(APPEND coek_compile_options -DWITH_GUROBI)
    list(APPEND coek_link_libraries gurobi_g++5.2 gurobi90)
endif()

#
# IPOPT LIBRARY
#
if(with_ipopt)
    if (ipopt_dir)
        MESSAGE("-- IPOPT_DIR=${ipopt_dir}")
        list(APPEND coek_include_directories ${ipopt_dir}/include/coin)
        list(APPEND coek_link_directories ${ipopt_dir}/lib ${ipopt_dir}/lib64)
        list(APPEND coek_link_libraries ipopt dl blas f2c)
    elseif(NOT "$ENV{IPOPT_HOME}" STREQUAL "")
        MESSAGE("-- IPOPT_HOME=$ENV{IPOPT_HOME}")
        list(APPEND coek_include_directories $ENV{IPOPT_HOME}/include/coin)
        list(APPEND coek_link_directories $ENV{IPOPT_HOME}/lib)
        list(APPEND coek_link_libraries ipopt dl blas gfortran)
    else()
        MESSAGE("-- IPOPT_HOME Not provided")
    endif()
    list(APPEND sources
                solvers/ipopt.cpp)
    list(APPEND coek_compile_options -DWITH_IPOPT)
endif()

#
# Solver Interface Options
#
#if(with_cffi)
#    list(APPEND sources
#        capi/coek_capi.cpp)
#        include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coek/capi)
#endif()

#
# Build library
#
add_library(coek SHARED ${sources})
target_compile_options(coek PUBLIC ${coek_compile_options})
target_include_directories(coek
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_include_directories(coek
                           PUBLIC
                           ${coek_include_directories})
if(${with_gcov})
  target_link_libraries(coek PUBLIC gcov)
endif()
set_property(TARGET coek PROPERTY INTERFACE_LINK_LIBRARIES ${coek_link_libraries})
set_property(TARGET coek PROPERTY INTERFACE_LINK_DIRECTORIES ${coek_link_directories})


#
# make install
#
install(TARGETS coek
        EXPORT coekTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
       )
install(FILES
        coek.hpp 
        coek_model.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek
       )
install(FILES
        api/expression.hpp
        api/expression_visitor.hpp
        api/intrinsic_fn.hpp
        api/objective.hpp
        api/constraint.hpp
        api/constants.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/api
       )
install(FILES
        compact/coek_sets.hpp
        compact/coek_indexed.hpp
        compact/sequence_context.hpp
        compact/expression_sequence.hpp
        compact/objective_sequence.hpp
        compact/constraint_sequence.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/compact
       )

#
# Push the compile options to the parent scope
#
set(coek_compile_options ${coek_compile_options} PARENT_SCOPE)
set(coek_link_libraries ${coek_link_libraries} PARENT_SCOPE)
set(coek_include_directories ${coek_include_directories} PARENT_SCOPE)
set(coek_link_directories ${coek_link_directories} PARENT_SCOPE)

