
##################### Build Shared Library  #####################

SET(sources
    util/index_vector.cpp
    util/option_cache.cpp
    util/tictoc.cpp
    ast/base_terms.cpp
    ast/constraint_terms.cpp
    ast/value_terms.cpp
    ast/expr_terms.cpp
    ast/visitor_to_list.cpp
    ast/visitor_write_expr.cpp
    ast/visitor_to_MutableNLPExpr.cpp
    ast/visitor_to_QuadraticExpr.cpp
    ast/visitor_symdiff.cpp
    ast/visitor_mutable_values.cpp
    ast/visitor_variables.cpp
    ast/visitor_simplify.cpp
    ast/visitor_eval.cpp
    #ast/varray.cpp
    api/constants.cpp
    api/expression.cpp
    api/expression_visitor.cpp
    api/objective.cpp
    api/constraint.cpp
    api/intrinsic_fn.cpp
    model/model.cpp
    model/compact_model.cpp
    model/nlp_model.cpp
    model/writer_lp.cpp
    model/writer_nl.cpp
    model/reader_jpof.cpp
    solvers/solver.cpp
    solvers/solver_repn.cpp
    solvers/testsolver.cpp
    autograd/autograd.cpp
    abstract/expr_rule.cpp
   )
if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    list(APPEND sources
        ast/compact_terms.cpp
        api/parameter_assoc_array.cpp
        api/parameter_array.cpp
        api/variable_assoc_array.cpp
        api/variable_array.cpp
        api/constraint_map.cpp
        api/subexpression_map.cpp
        util/DataPortal.cpp
        )
endif()

if(with_compact)
    list(APPEND sources
        compact/sequence_context.cpp
        compact/expression_sequence.cpp
        compact/variable_sequence.cpp
        compact/objective_sequence.cpp
        compact/constraint_sequence.cpp
        compact/ast_set.cpp
        compact/coek_sets.cpp
        #compact/coek_indexed.cpp
        compact/parameter_map.cpp
        compact/variable_map.cpp
        compact/visitor_exprtemplate.cpp
        )
endif()

# Dynamic linking options
list(APPEND sources util/loadlib.cpp)
if(WIN32)
else()
    list(APPEND coek_compile_options -DUSING_DL)
    list(APPEND coek_link_libraries dl)
endif()

# fmtlib LIBRARY
if(with_fmtlib)
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

if(with_rapidjson)
    find_package(RapidJSON REQUIRED)
    list(APPEND coek_compile_options -DWITH_RAPIDJSON)
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include)
endif()

# gcov
if(with_gcov)
    list(APPEND coek_compile_options -fprofile-arcs -ftest-coverage -g)
    list(APPEND coek_link_libraries gcov)
endif()

# gprof
if (MSVC)
else()
    if(${with_gprof})
        list(APPEND coek_compile_options "-pg")
    else()
        list(APPEND coek_compile_options "-Wall;-O3")
    endif()
endif()

#
# ADMODEL OPTIONS
#

# CppAD LIBRARY
if(with_cppad)
    list(APPEND sources
                autograd/cppad_repn.cpp)
    list(APPEND coek_compile_options "-DWITH_CPPAD")
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include)
endif()

# ASL LIBRARY
if(with_asl)
    list(APPEND sources
                autograd/asl_repn.cpp)
    list(APPEND coek_compile_options "-DWITH_ASL")
    list(APPEND coek_include_directories ${CMAKE_INSTALL_PREFIX}/include/asl)
    list(APPEND coek_link_directories ${CMAKE_INSTALL_PREFIX}/lib)
    if (${with_openmp})
        list(APPEND coek_link_libraries asl-mt)
        list(APPEND coek_link_libraries OpenMP::OpenMP_CXX)
    else()
        list(APPEND coek_link_libraries asl)
    endif()
endif()

#
# SOLVER OPTIONS
#

#
# GUROBI LIBRARY
#
if(with_gurobi)
    list(APPEND sources
                solvers/gurobi/gurobi.cpp)
    #if(NOT "$ENV{GUROBI_HOME}" STREQUAL "")
    #    MESSAGE("-- GUROBI_HOME $ENV{GUROBI_HOME}")
    #    list(APPEND coek_link_directories $ENV{GUROBI_HOME}/lib)
    #else()
    #    MESSAGE("-- GUROBI_HOME Not provided")
    #endif()
    list(APPEND coek_compile_options -DWITH_GUROBI)
    list(APPEND coek_link_libraries ${GUROBI_CXX_LIBRARY} ${GUROBI_LIBRARY})
    list(APPEND coek_include_directories ${GUROBI_INCLUDE_DIRS})
#gurobi_g++5.2 gurobi90)
endif()

#
# IPOPT LIBRARY
#
list(APPEND sources
        solvers/ipopt/ipopt_capi.cpp
        solvers/ipopt/ipopt_solver.cpp)

#
# Solver Interface Options
#
#if(with_cffi)
#    list(APPEND sources
#        capi/coek_capi.cpp)
#        include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coek/capi)
#endif()

#
# Build object library
#
# this is the "object library" target: compiles the sources only once
if (build_shared_libs)
    add_library(coek SHARED ${sources})
    message("-- Building shared libcoek library")
    set_target_properties(coek PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
else()
    add_library(coek STATIC ${sources})
    message("-- Building static libcoek library")
endif()
add_library(coek::coek ALIAS coek)

target_compile_options(coek PUBLIC ${coek_compile_options})
target_include_directories(coek
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                           )
target_include_directories(coek
                           PUBLIC
                           ${coek_include_directories})
target_link_libraries(coek PUBLIC ${coek_link_libraries})
target_link_directories(coek PUBLIC ${coek_link_directories})
set_property(TARGET coek PROPERTY INTERFACE_LINK_LIBRARIES ${coek_link_libraries})

#if (WIN32)
#    target_link_libraries(coek ${coek_link_libraries})
#    target_link_directories(coek PRIVATE ${coek_link_directories})
#endif()

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
if (WIN32)
    set_target_properties(coek PROPERTIES OUTPUT_NAME libcoek)
endif()

install(FILES
        coek.hpp 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek
        )
install(FILES
        util/index_vector.hpp
        util/template_utils.hpp
        util/sequence.hpp
        util/option_cache.hpp
        util/DataPortal.hpp
        util/tictoc.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/util
        )
install(FILES
        api/constraint_map.hpp
        api/subexpression_map.hpp
        api/constants.hpp
        api/constraint.hpp
        api/expression.hpp
        api/expression_visitor.hpp
        api/intrinsic_fn.hpp
        api/indexed_container.hpp
        api/objective.hpp
        api/parameter_array.hpp
        api/parameter_assoc_array.hpp
        api/parameter_assoc_array_repn.hpp
        api/variable_array.hpp
        api/variable_assoc_array.hpp
        api/variable_assoc_array_repn.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/api
        )
install(FILES
        compact/coek_sets.hpp
        compact/constraint_sequence.hpp
        compact/expression_sequence.hpp
        compact/objective_sequence.hpp
        compact/sequence_context.hpp
        compact/parameter_map.hpp
        compact/variable_map.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/compact
        )
install(FILES
        model/model.hpp
        model/nlp_model.hpp
        model/compact_model.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/model
        )
install(FILES
        solvers/solver.hpp
        solvers/solver_results.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coek/solvers
        )

#
# Push the compile options to the parent scope
#
set(coek_compile_options ${coek_compile_options} PARENT_SCOPE)
set(coek_link_libraries ${coek_link_libraries} PARENT_SCOPE)
set(coek_include_directories ${coek_include_directories} PARENT_SCOPE)
set(coek_link_directories ${coek_link_directories} PARENT_SCOPE)
