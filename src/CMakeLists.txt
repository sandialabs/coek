
##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    coek_capi.cpp
    expr/expr_types.cpp
    expr/expr_manager_objects.cpp
    model/simple.cpp
    solver.cpp
    admodel/simple.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- Building Source with CppAd Library: on")
  list(APPEND sources
    admodel/cppad.cpp)
  include_directories(BEFORE ${CPPAD_INCLUDE_DIR})
else()
  MESSAGE("-- Building Source with CppAd Library: off")
endif()

# ASL LIBRARY
if(with_asl)
  MESSAGE("-- With ASL Library: on")
  list(APPEND sources
	autograd/asl_model.cpp)
else()
  MESSAGE("-- With ASL Library: off")
endif()

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

