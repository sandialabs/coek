
##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    coek_capi.cpp
    expr/expr_types.cpp
    expr/expr_manager_objects.cpp
    solver.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

#
# EXPRMODEL OPTIONS
#

MESSAGE("-- Building Source with ExprModel: Default")
list(APPEND sources exprmodel/exprmodel_default.cpp)

#
# ADMODEL OPTIONS
#

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Source with AD:        CppAd")
  list(APPEND sources
    admodel/cppad.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
endif()

# ASL LIBRARY
if(NOT with_cppad AND with_asl)
  MESSAGE("-- Building Source with AD:        ASL")
  list(APPEND sources
	autograd/asl_model.cpp)
endif()

# Default AD LIBRARY
if(NOT with_cppad AND NOT with_asl)
  MESSAGE("-- Building Source with AD:        Simple")
  list(APPEND sources
    admodel/simplead.cpp)
  add_definitions(-DWITH_ADMODEL_SIMPLEAD)
endif()

#
# SOLVER OPTIONS
#

# GUROBI LIBRARY
if(${GUROBI_HOME})
    MESSAGE("-- GUROBI_HOME ${GUROBI_HOME}")
    INCLUDE_DIRECTORIES(${GUROBI_HOME}/include)
else()
    MESSAGE("-- GUROBI_HOME Not provided")
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

# build library
ADD_LIBRARY(coek SHARED ${sources})
TARGET_COMPILE_OPTIONS(coek PRIVATE ${solver_flags})
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(coek PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(coek PUBLIC gcov)
endif()


# make install
INSTALL(TARGETS coek
        DESTINATION lib
       )
INSTALL(FILES coek_capi.h
        DESTINATION include
       )

