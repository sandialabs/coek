cmake_minimum_required(VERSION 3.7)

# Use pybind11 to create pycoek Python module
#
# NOTE: we probably need to extend this to link in solver libraries
#

include_directories("${PROJECT_SOURCE_DIR}/src/")
include_directories("${PROJECT_SOURCE_DIR}/src/expr/")
if(${with_debug})
    pybind11_add_module(pycoek NO_EXTRAS pycoek.cpp)
else()
    pybind11_add_module(pycoek pycoek.cpp)
endif()
TARGET_LINK_LIBRARIES(pycoek PUBLIC coek)
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(pycoek PUBLIC ipopt gfortran)
endif()
if(${with_gurobi})
  TARGET_LINK_LIBRARIES(pycoek PUBLIC gurobi_g++5.2 gurobi81)
endif()

INSTALL(TARGETS pycoek
        DESTINATION lib)

