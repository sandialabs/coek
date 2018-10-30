
##################### Build Shared Library  #####################

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    expr_api.cpp
    expr_api.h
    expr_types.cpp
    context_objects.cpp
    model.cpp
    solver.cpp
    autograd/simple_model.cpp
   )

# CppAD LIBRARY
if(with_cppad)
  MESSAGE("-- With CppAd Library: on")
  list(APPEND sources
	autograd/cppad_model.cpp)
else()
  MESSAGE("-- With CppAd Library: off")
endif()

# ASL LIBRARY
if(with_asl)
  MESSAGE("-- With ASL Library: on")
  list(APPEND sources
	autograd/asl_model.cpp)
else()
  MESSAGE("-- With ASL Library: off")
endif()

# build library
ADD_LIBRARY(coek SHARED ${sources})
if(${GUROBI_HOME})
    MESSAGE("-- GUROBI_HOME ${GUROBI_HOME}")
    TARGET_INCLUDE_DIRECTORIES(coek ${GUROBI_HOME}/include)
else()
    MESSAGE("-- GUROBI_HOME Not provided")
endif()
TARGET_COMPILE_OPTIONS(coek PRIVATE ${solver_flags})
if(${with_gcov})
  TARGET_COMPILE_OPTIONS(coek PUBLIC -fprofile-arcs -ftest-coverage -g)
  TARGET_LINK_LIBRARIES(coek PUBLIC gcov)
endif()


# make install
INSTALL(TARGETS coek
        DESTINATION lib
       )
INSTALL(FILES expr_api.h
        DESTINATION include
       )
INSTALL(FILES expr_intrinsic.h
        DESTINATION include
       )

